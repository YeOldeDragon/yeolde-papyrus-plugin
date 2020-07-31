#include "YeOldeGameData.h"
#include "YeOldeFormList.h"
#include "YeOldeForm.h"
#include "WMRResultArrayUtils.h"

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
			if (baseEnchant != NULL && !VMResultArrayUtils::HasItem(&result, (TESForm*) baseEnchant))
			{
				result.push_back(baseEnchant);
			}
		}

		_MESSAGE("GetAllBaseEnchantments() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllCookingIngredients(StaticFunctionTag*)
	{
		_MESSAGE("GetAllCookingIngredients() -> START");
		VMResultArray<TESForm*> result;

		DataHandler* dataHandler = DataHandler::GetSingleton();
		AlchemyItem* item = NULL;
		_MESSAGE("GetAllCookingIngredients() -> Alchemy items count: %u", dataHandler->potions.count);
		for (UInt32 i = 0; i < dataHandler->potions.count; i++)
		{
			dataHandler->potions.GetNthItem(i, item);
			if (YeOldeForm::HasKeywords(item, "VendorItemFood") || YeOldeForm::HasKeywords(item, "VendorItemFoodRaw"))
				result.push_back(item);
		}

		_MESSAGE("GetAllCookingIngredients() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllAlchemyIngredients(StaticFunctionTag*)
	{
		_MESSAGE("GetAllIngredients() -> START");
		VMResultArray<TESForm*> result;

		DataHandler* dataHandler = DataHandler::GetSingleton();
		IngredientItem* ingredient = NULL;
		_MESSAGE("GetAllIngredients() -> Ingredients count: %u", dataHandler->ingredients.count);
		for (UInt32 i = 0; i < dataHandler->ingredients.count; i++)
		{
			dataHandler->ingredients.GetNthItem(i, ingredient);
			_MESSAGE("  -> ingredient: %u, %x, %u", ingredient->formID, ingredient->formID, ingredient->IsPlayable());
			result.push_back(ingredient);
		}

		_MESSAGE("GetAllIngredients() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllSoulGems(StaticFunctionTag*, bool filledOnly)
	{
		_MESSAGE("GetAllSoulGems() -> START");
		VMResultArray<TESForm*> result;
		DataHandler* dataHandler = DataHandler::GetSingleton();
		tArray<TESForm*> gems = dataHandler->arrSLGM;

		for (UInt32 i = 0; i < gems.count; i++)
		{
			TESSoulGem* gem = (TESSoulGem*)gems[i];

			if (!gem || (filledOnly && gem->soulSize == 0))
				continue;

			result.push_back(gem);
		}
		_MESSAGE("GetAllSoulGems() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllSoulGems(bool filledOnly)
	{
		return GetAllSoulGems(NULL, filledOnly);
	}

	VMResultArray<TESForm*> GetAllTanningMaterials(StaticFunctionTag*)
	{
		_MESSAGE("GetAllTanningIngredients() -> START");
		VMResultArray<TESForm*> result;

		DataHandler* dataHandler = DataHandler::GetSingleton();
		TESObjectMISC* item = NULL;
		_MESSAGE("GetAllTanningIngredients() -> Misc objects count: %u", dataHandler->miscObjects.count);
		for (UInt32 i = 0; i < dataHandler->miscObjects.count; i++)
		{
			dataHandler->miscObjects.GetNthItem(i, item);
			if (YeOldeForm::HasKeywords(item, "VendorItemAnimalHide"))
				result.push_back(item);
		}

		_MESSAGE("GetAllTanningIngredients() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllOreIngotMaterials(StaticFunctionTag*)
	{
		_MESSAGE("GetAllOreIngotMaterials() -> START");
		VMResultArray<TESForm*> result;

		DataHandler* dataHandler = DataHandler::GetSingleton();
		TESObjectMISC* item = NULL;
		_MESSAGE("GetAllOreIngotMaterials() -> Misc objects count: %u", dataHandler->miscObjects.count);
		for (UInt32 i = 0; i < dataHandler->miscObjects.count; i++)
		{
			dataHandler->miscObjects.GetNthItem(i, item);
			if (YeOldeForm::HasKeywords(item, "VendorItemOreIngot"))
				result.push_back(item);
		}

		_MESSAGE("GetAllOreIngotMaterials() -> END");
		return result;
	}

	VMResultArray<TESForm*> GetAllMaterialsFromConstructibleObject(StaticFunctionTag*)
	{
		_MESSAGE("GetAllMaterialsFromConstructibleObject() -> START");

		DataHandler* dataHandler = DataHandler::GetSingleton();
		tArray<TESForm*> arrCOBJ = dataHandler->arrCOBJ;
		VMResultArray<TESForm*> result;
		UInt32 nbItems = 0;

		_MESSAGE("GetAllMaterialsFromConstructibleObject() -> %u Constructible objects found", arrCOBJ.count);
		for (UInt32 i = 0; i < arrCOBJ.count; i++)
		{
			BGSConstructibleObject* recipe = (BGSConstructibleObject*)arrCOBJ[i];

			if (!recipe || !recipe->createdObject)
				continue;

			for (UInt32 j = 0; j < recipe->container.numEntries; j++)
			{
				TESForm* material = recipe->container.entries[j]->form;

				if (material == NULL)
					continue;

				if (IsNotMaterial(material))
					continue;

				if (YeOldeForm::HasKeywords(material, "VendorItemOreIngot") ||
					YeOldeForm::HasKeywords(material, "VendorItemAnimalHide"))
					continue;

				// If the item is already in the list, no need to add it.
				if (VMResultArrayUtils::HasItem(&result, material))
					continue;

				nbItems++;
				result.push_back(material);
			}
		}

		_MESSAGE("GetAllMaterialsFromConstructibleObject() -> Total items returned:  %u", nbItems);
		return result;
	}


	void AddAllBaseEnchantmentsToList(StaticFunctionTag*, BGSListForm* result)
	{
		VMResultArray<TESForm*> items = GetAllBaseEnchantments();
		YeOldeFormList::AddForms(result, &items);
	}

	void AddAllCookingIngredientsToList(StaticFunctionTag*, BGSListForm* result)
	{
		VMResultArray<TESForm*> items = GetAllCookingIngredients();
		YeOldeFormList::AddForms(result, &items);
	}

	void AddAllAlchemyIngredientsToList(StaticFunctionTag*, BGSListForm* result)
	{
		VMResultArray<TESForm*> items = GetAllAlchemyIngredients();
		YeOldeFormList::AddForms(result, &items);
	}

	void AddAllSoulGemsToList(StaticFunctionTag*, BGSListForm* result, bool filledOnly)
	{
		VMResultArray<TESForm*> items = GetAllSoulGems();
		YeOldeFormList::AddForms(result, &items);
	}

	void AddAllTanningMaterialsToList(StaticFunctionTag*, BGSListForm* result)
	{
		VMResultArray<TESForm*> items = GetAllTanningMaterials();
		YeOldeFormList::AddForms(result, &items);
	}

	void AddAllOreIngotMaterialsToList(StaticFunctionTag*, BGSListForm* result)
	{
		VMResultArray<TESForm*> items = GetAllOreIngotMaterials();
		YeOldeFormList::AddForms(result, &items);
	}

	void AddAllCraftingMaterialsToList(StaticFunctionTag*, BGSListForm* craftingList, bool addAlchemy, bool addCooking, bool addOreIngots, bool addSoulgems, bool addTanning, bool addFromCOBJ)
	{
		_MESSAGE("UpdateCraftingMaterialsList -> START");
		if (craftingList == NULL)
		{
			_ERROR("UpdateCraftingMaterialsList -> craftingList is NULL.");
			return;
		}
		VMResultArray<TESForm*>* result = new VMResultArray<TESForm*>();
		VMResultArray<TESForm*> tmpResult;

		if (addAlchemy)
		{
			tmpResult = YeOldeGameData::GetAllAlchemyIngredients();
			VMResultArrayUtils::Add(result, tmpResult);
		}

		if (addCooking)
		{
			tmpResult = YeOldeGameData::GetAllCookingIngredients();
			VMResultArrayUtils::Add(result, tmpResult);
		}

		if (addOreIngots)
		{
			tmpResult = YeOldeGameData::GetAllOreIngotMaterials();
			VMResultArrayUtils::Add(result, tmpResult);
		}

		if (addSoulgems)
		{
			tmpResult = YeOldeGameData::GetAllSoulGems();
			VMResultArrayUtils::Add(result, tmpResult);
		}

		if (addTanning)
		{
			tmpResult = YeOldeGameData::GetAllTanningMaterials();
			VMResultArrayUtils::Add(result, tmpResult);
		}

		if (addFromCOBJ)
		{
			tmpResult = YeOldeGameData::GetAllMaterialsFromConstructibleObject();
			VMResultArrayUtils::Add(result, tmpResult);
		}

		_MESSAGE("UpdateCraftingMaterialsList -> result contains %u items)", result->size());
		YeOldeFormList::AddForms(craftingList, result);
		_MESSAGE("UpdateCraftingMaterialsList -> END (%u items returned)", craftingList->forms.count);
	}


	void ConcatenateFormLists(StaticFunctionTag*, BGSListForm* result, BGSListForm* itemsToAdd)
	{
		YeOldeFormList::ConcatenateLists(result, itemsToAdd);
	}


	bool RegisterFuncs(VMClassRegistry* registry)
	{
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllBaseEnchantments", "yeolde_papyrus", YeOldeGameData::GetAllBaseEnchantments, registry));
	
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllCookingIngredients", "yeolde_papyrus", YeOldeGameData::GetAllCookingIngredients, registry));
	
		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllAlchemyIngredients", "yeolde_papyrus", YeOldeGameData::GetAllAlchemyIngredients, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, VMResultArray<TESForm*>, bool>("GetAllSoulGems", "yeolde_papyrus", YeOldeGameData::GetAllSoulGems, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllTanningMaterials", "yeolde_papyrus", YeOldeGameData::GetAllTanningMaterials, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllOreIngotMaterials", "yeolde_papyrus", YeOldeGameData::GetAllOreIngotMaterials, registry));

		registry->RegisterFunction(
			new NativeFunction0 <StaticFunctionTag, VMResultArray<TESForm*>>("GetAllMaterialsFromConstructibleObject", "yeolde_papyrus", YeOldeGameData::GetAllMaterialsFromConstructibleObject, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSListForm*>("AddAllBaseEnchantmentsToList", "yeolde_papyrus", YeOldeGameData::AddAllBaseEnchantmentsToList, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSListForm*>("AddAllCookingIngredientsToList", "yeolde_papyrus", YeOldeGameData::AddAllCookingIngredientsToList, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSListForm*>("AddAllAlchemyIngredientsToList", "yeolde_papyrus", YeOldeGameData::AddAllAlchemyIngredientsToList, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BGSListForm*, bool>("AddAllSoulGemsToList", "yeolde_papyrus", YeOldeGameData::AddAllSoulGemsToList, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSListForm*>("AddAllOreIngotMaterialsToList", "yeolde_papyrus", YeOldeGameData::AddAllOreIngotMaterialsToList, registry));

		registry->RegisterFunction(
			new NativeFunction1 <StaticFunctionTag, void, BGSListForm*>("AddAllTanningMaterialsToList", "yeolde_papyrus", YeOldeGameData::AddAllTanningMaterialsToList, registry));

		registry->RegisterFunction(
			new NativeFunction7 <StaticFunctionTag, void, BGSListForm*, bool, bool, bool, bool, bool, bool>("AddAllCraftingMaterialsToList", "yeolde_papyrus", YeOldeGameData::AddAllCraftingMaterialsToList, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BGSListForm*, BGSListForm*>("ConcatenateFormLists", "yeolde_papyrus", YeOldeGameData::ConcatenateFormLists, registry));

		return true;
	}
}
