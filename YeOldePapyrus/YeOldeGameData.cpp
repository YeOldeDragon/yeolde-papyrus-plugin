#include "YeOldeGameData.h"
#include "YeOldeFormList.h"
#include "YeOldeForm.h"

#include "skse64/GameFormComponents.h"
#include "skse64/GameData.h"
#include "skse64/GameRTTI.h"
#include "skse64/GameForms.h"
#include "skse64/GameObjects.h"
#include "skse64/GameReferences.h"

#include "skse64/PapyrusVM.h"
#include "skse64/PapyrusNativeFunctions.h"

#include "skse64/PapyrusArgs.h"
#include "skse64/PapyrusKeyword.h"

namespace YeOldeGameData
{

	bool IsNotMaterial(TESForm* material)
	{
		if (material->formType == FormType::kFormType_Ingredient || 
			material->formType == FormType::kFormType_SoulGem ||
			material->formType == FormType::kFormType_Potion ||
			material->formType == FormType::kFormType_Weapon ||
			material->formType == FormType::kFormType_Armor ||
			material->formType == FormType::kFormType_Book ||
			material->formType == FormType::kFormType_Ammo ||
			material->formType == FormType::kFormType_LeveledItem ||
			material->formType == FormType::kFormType_Outfit)
			return true;

		return false;
	}

	VMResultArray<TESForm*> GetAllBaseEnchantments(StaticFunctionTag*)
	{
		_MESSAGE("GetAllBaseEnchantments() -> START");
		VMResultArray<TESForm*> result;

		DataHandler* dataHandler = DataHandler::GetSingleton();

		EnchantmentItem* enchant = NULL;
		EnchantmentItem* baseEnchant = NULL;

		_MESSAGE("GetAllBaseEnchantments() -> Enchantments count: %u", dataHandler->enchantments.count);
		for (UInt32 i = 0; i < dataHandler->enchantments.count; i++)
		{
			dataHandler->enchantments.GetNthItem(i, enchant);

			baseEnchant = enchant->data.baseEnchantment;

			// If there is a base enchantment and we didn't already pushed it in the result
			if (baseEnchant != NULL && std::find(result.begin(), result.end(), baseEnchant) == result.end())
			{
				result.push_back(baseEnchant);
			}
		}

		_MESSAGE("GetAllBaseEnchantments() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllIngredients(StaticFunctionTag*)
	{
		_MESSAGE("GetAllIngredients() -> START");
		VMResultArray<TESForm*> result;

		DataHandler* dataHandler = DataHandler::GetSingleton();
		IngredientItem* ingredient = NULL;
		_MESSAGE("GetAllIngredients() -> Ingredients count: %u", dataHandler->ingredients.count);
		for (UInt32 i = 0; i < dataHandler->ingredients.count; i++)
		{
			dataHandler->ingredients.GetNthItem(i, ingredient);
			result.push_back(ingredient);
		}

		_MESSAGE("GetAllIngredients() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllSoulGems()
	{
		_MESSAGE("GetAllSoulGems() -> START");
		VMResultArray<TESForm*> result;
		DataHandler* dataHandler = DataHandler::GetSingleton();
		tArray<TESForm*> gems = dataHandler->arrSLGM;

		for (UInt32 i = 0; i < gems.count; i++)
		{
			TESSoulGem* gem = (TESSoulGem*)gems[i];

			if (!gem || gem->soulSize == 0)
				continue;

			result.push_back(gem);
		}
		_MESSAGE("GetAllSoulGems() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllMaterialsFromConstructibleObject(bool addIngots, bool addOthers)
	{
		_MESSAGE("GetAllMaterialsFromConstructibleObject(%i, %i) -> START", addIngots, addOthers);
		VMResultArray<TESForm*> result;

		DataHandler* dataHandler = DataHandler::GetSingleton();
		tArray<TESForm*> arrCOBJ = dataHandler->arrCOBJ;
		bool isIngot = false;
		bool isSoulgem = false;
		UInt32 nbItems = 0;

		_MESSAGE("GetAllCraftingMaterials() -> %u Constructible objects found", arrCOBJ.count);
		for (UInt32 i = 0; i < arrCOBJ.count; i++)
		{
			BGSConstructibleObject* recipe = (BGSConstructibleObject * ) arrCOBJ[i];

			if (!recipe || !recipe->createdObject)
				continue;

			for (UInt32 j = 0; j < recipe->container.numEntries; j++)
			{
				TESForm* material = recipe->container.entries[j]->form;
				
				if (material == NULL)
					continue;

				if (YeOldeForm::HasKeywords(material, "VendorItemOreIngot") && !addIngots)
					continue;

				if (!isIngot && !addOthers)
					continue;

				if (IsNotMaterial(material))
					continue;

				// If the item is already in the list, no need to add it.
				if (std::find(result.begin(), result.end(), material) != result.end())
					continue;
				nbItems++;
				result.push_back(material);
			}
		}

		_MESSAGE("GetAllCraftingMaterials() -> Total items returned:  %u", nbItems);
		return result;
	}

	void UpdateCraftingMaterialsList(StaticFunctionTag*, BGSListForm* craftingList, bool addIngredients, bool addIngots, bool addSoulgems, bool addOthers)
	{
		_MESSAGE("UpdateCraftingMaterialsList -> START");
		if (craftingList == NULL) 
		{
			_ERROR("UpdateCraftingMaterialsList -> craftingList is NULL.");
			return;
		}
		VMResultArray<TESForm*> result;
		VMResultArray<TESForm*> tmpResult;

		if (addIngots || addOthers)
		{
			tmpResult = GetAllMaterialsFromConstructibleObject(addIngots, addOthers);
			YeOldeFormList::AddForms(craftingList, tmpResult);
		}

		if (addSoulgems)
		{
			tmpResult = GetAllSoulGems();
			YeOldeFormList::AddForms(craftingList, tmpResult);
		}

		if (addIngredients)
		{
			tmpResult = GetAllIngredients(NULL);
			YeOldeFormList::AddForms(craftingList, tmpResult);
		}
		_MESSAGE("UpdateCraftingMaterialsList -> END");
	}
}

bool YeOldeGameData::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllBaseEnchantments", "yeolde_papyrus_utils", YeOldeGameData::GetAllBaseEnchantments, registry));
	
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllIngredients", "yeolde_papyrus_utils", YeOldeGameData::GetAllIngredients, registry));

	registry->RegisterFunction(
		new NativeFunction5 <StaticFunctionTag, void, BGSListForm*, bool, bool, bool, bool>("UpdateCraftingMaterialsList", "yeolde_papyrus_utils", YeOldeGameData::UpdateCraftingMaterialsList, registry));
	
	return true;
}
