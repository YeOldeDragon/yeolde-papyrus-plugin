#include "CraftingData.h"

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

#include "YeOldeFormList.h"
#include "YeOldeForm.h"
#include "WMRResultArrayUtils.h"



void CraftingData::Initialize()
{
	_badMaterialTypes.push_back(FormType::kFormType_Ingredient);
	_badMaterialTypes.push_back(FormType::kFormType_SoulGem);
	_badMaterialTypes.push_back(FormType::kFormType_Potion);
	_badMaterialTypes.push_back(FormType::kFormType_Weapon);
	_badMaterialTypes.push_back(FormType::kFormType_Armor);
	_badMaterialTypes.push_back(FormType::kFormType_Book);
	_badMaterialTypes.push_back(FormType::kFormType_Ammo);
	_badMaterialTypes.push_back(FormType::kFormType_LeveledItem);
	_badMaterialTypes.push_back(FormType::kFormType_Outfit);

}

VMResultArray<TESForm*> CraftingData::GetAllBaseEnchantments()
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
		if (baseEnchant != NULL && !VMResultArrayUtils::HasItem(&result, (TESForm*)baseEnchant) && !IsBlackListed(baseEnchant))
			result.push_back(baseEnchant);
	}

	_MESSAGE("GetAllBaseEnchantments() -> END");
	return result;
}

VMResultArray<TESForm*> CraftingData::GetAllCookingIngredients()
{
	_MESSAGE("GetAllCookingIngredients() -> START");
	VMResultArray<TESForm*> result;

	DataHandler* dataHandler = DataHandler::GetSingleton();
	AlchemyItem* item = NULL;
	_MESSAGE("GetAllCookingIngredients() -> Alchemy items count: %u", dataHandler->potions.count);
	for (UInt32 i = 0; i < dataHandler->potions.count; i++)
	{
		dataHandler->potions.GetNthItem(i, item);
		if (YeOldeForm::HasKeywords(item, "VendorItemFood") || YeOldeForm::HasKeywords(item, "VendorItemFoodRaw") && !IsBlackListed(item))
			result.push_back(item);
	}

	_MESSAGE("GetAllCookingIngredients() -> END");
	return result;
}

VMResultArray<TESForm*> CraftingData::GetAllAlchemyIngredients()
{
	_MESSAGE("GetAllIngredients() -> START");
	VMResultArray<TESForm*> result;

	DataHandler* dataHandler = DataHandler::GetSingleton();
	IngredientItem* ingredient = NULL;
	_MESSAGE("GetAllIngredients() -> Ingredients count: %u", dataHandler->ingredients.count);
	for (UInt32 i = 0; i < dataHandler->ingredients.count; i++)
	{
		dataHandler->ingredients.GetNthItem(i, ingredient);
		if (IsBlackListed(ingredient))
			continue;

		//_MESSAGE("  -> ingredient: %u, %x, %u", ingredient->formID, ingredient->formID, ingredient->IsPlayable());
		result.push_back(ingredient);
	}

	_MESSAGE("GetAllIngredients() -> END");
	return result;
}

VMResultArray<TESForm*> CraftingData::GetAllSoulGems(bool filledOnly)
{
	_MESSAGE("GetAllSoulGems() -> START");
	VMResultArray<TESForm*> result;
	DataHandler* dataHandler = DataHandler::GetSingleton();
	tArray<TESForm*> gems = dataHandler->arrSLGM;

	for (UInt32 i = 0; i < gems.count; i++)
	{
		TESSoulGem* gem = (TESSoulGem*)gems[i];

		//_MESSAGE("GetAllSoulGems() -> gem %u: soulSize=%u, gemSize=%u", dataHandler->miscObjects.count, gem->soulSize, gem->gemSize);
		if (!gem || (filledOnly && gem->soulSize == 0) || IsBlackListed(gem))
			continue;

		result.push_back(gem);
	}
	_MESSAGE("GetAllSoulGems() -> END");
	return result;
}

VMResultArray<TESForm*> CraftingData::GetAllTanningMaterials()
{
	_MESSAGE("GetAllTanningIngredients() -> START");
	VMResultArray<TESForm*> result;

	DataHandler* dataHandler = DataHandler::GetSingleton();
	TESObjectMISC* item = NULL;
	_MESSAGE("GetAllTanningIngredients() -> Misc objects count: %u", dataHandler->miscObjects.count);
	for (UInt32 i = 0; i < dataHandler->miscObjects.count; i++)
	{
		dataHandler->miscObjects.GetNthItem(i, item);
		if (YeOldeForm::HasKeywords(item, "VendorItemAnimalHide") && !IsBlackListed(item))
			result.push_back(item);
	}

	_MESSAGE("GetAllTanningIngredients() -> END");
	return result;
}

VMResultArray<TESForm*> CraftingData::GetAllOreIngotMaterials()
{
	_MESSAGE("GetAllOreIngotMaterials() -> START");
	VMResultArray<TESForm*> result;

	DataHandler* dataHandler = DataHandler::GetSingleton();
	TESObjectMISC* item = NULL;
	_MESSAGE("GetAllOreIngotMaterials() -> Misc objects count: %u", dataHandler->miscObjects.count);
	for (UInt32 i = 0; i < dataHandler->miscObjects.count; i++)
	{
		dataHandler->miscObjects.GetNthItem(i, item);
		if (YeOldeForm::HasKeywords(item, "VendorItemOreIngot") && !IsBlackListed(item))
			result.push_back(item);
	}

	_MESSAGE("GetAllOreIngotMaterials() -> END");
	return result;
}

VMResultArray<TESForm*> CraftingData::GetAllMaterialsFromConstructibleObject()
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

			if (IsBadMaterial(material))
				continue;

			if (YeOldeForm::HasKeywords(material, "VendorItemOreIngot") ||
				YeOldeForm::HasKeywords(material, "VendorItemAnimalHide"))
				continue;

			// If the item is already in the list, no need to add it.
			if (VMResultArrayUtils::HasItem(&result, material))
				continue;

			if (IsBlackListed(material))
				continue;

			nbItems++;
			result.push_back(material);
		}
	}

	_MESSAGE("GetAllMaterialsFromConstructibleObject() -> Total items returned:  %u", nbItems);
	return result;
}


void CraftingData::AddAllBaseEnchantmentsToList(BGSListForm* result)
{
	VMResultArray<TESForm*> items = GetAllBaseEnchantments();
	YeOldeFormList::AddForms(result, &items);
}

void CraftingData::AddAllCookingIngredientsToList(BGSListForm* result)
{
	VMResultArray<TESForm*> items = GetAllCookingIngredients();
	YeOldeFormList::AddForms(result, &items);
}

void CraftingData::AddAllAlchemyIngredientsToList(BGSListForm* result)
{
	VMResultArray<TESForm*> items = GetAllAlchemyIngredients();
	YeOldeFormList::AddForms(result, &items);
}

void CraftingData::AddAllSoulGemsToList(BGSListForm* result, bool filledOnly)
{
	VMResultArray<TESForm*> items = GetAllSoulGems();
	YeOldeFormList::AddForms(result, &items);
}

void CraftingData::AddAllTanningMaterialsToList(BGSListForm* result)
{
	VMResultArray<TESForm*> items = GetAllTanningMaterials();
	YeOldeFormList::AddForms(result, &items);
}

void CraftingData::AddAllOreIngotMaterialsToList(BGSListForm* result)
{
	VMResultArray<TESForm*> items = GetAllOreIngotMaterials();
	YeOldeFormList::AddForms(result, &items);
}

void CraftingData::AddAllCraftingMaterialsToList(BGSListForm* craftingList, bool addAlchemy, bool addCooking, bool addOreIngots, bool addSoulgems, bool addTanning, bool addFromCOBJ)
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
		tmpResult = GetAllAlchemyIngredients();
		VMResultArrayUtils::Add(result, tmpResult);
	}

	if (addCooking)
	{
		tmpResult = GetAllCookingIngredients();
		VMResultArrayUtils::Add(result, tmpResult);
	}

	if (addOreIngots)
	{
		tmpResult = GetAllOreIngotMaterials();
		VMResultArrayUtils::Add(result, tmpResult);
	}

	if (addSoulgems)
	{
		tmpResult = GetAllSoulGems();
		VMResultArrayUtils::Add(result, tmpResult);
	}

	if (addTanning)
	{
		tmpResult = GetAllTanningMaterials();
		VMResultArrayUtils::Add(result, tmpResult);
	}

	if (addFromCOBJ)
	{
		tmpResult = GetAllMaterialsFromConstructibleObject();
		VMResultArrayUtils::Add(result, tmpResult);
	}

	_MESSAGE("UpdateCraftingMaterialsList -> result contains %u items)", result->size());
	YeOldeFormList::AddForms(craftingList, result);
	_MESSAGE("UpdateCraftingMaterialsList -> END (%u items returned)", craftingList->forms.count);
}