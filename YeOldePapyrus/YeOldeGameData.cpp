#include "YeOldeGameData.h"
#include "CraftingData.h"
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

#include "YeOldeFormList.h"
#include "YeOldeForm.h"

namespace YeOldeGameData
{
	VMResultArray<TESForm*> GetAllBaseEnchantments(StaticFunctionTag*)
	{
		auto data = new CraftingData();
		return data->GetAllBaseEnchantments();
	}

	VMResultArray<TESForm*> GetAllCookingIngredients(StaticFunctionTag*)
	{
		auto data = new CraftingData();
		return data->GetAllCookingIngredients();
	}

	VMResultArray<TESForm*> GetAllAlchemyIngredients(StaticFunctionTag*)
	{
		auto data = new CraftingData();
		return data->GetAllAlchemyIngredients();
	}

	VMResultArray<TESForm*> GetAllSoulGems(StaticFunctionTag*, bool filledOnly)
	{
		auto data = new CraftingData();
		return data->GetAllSoulGems();
	}

	VMResultArray<TESForm*> GetAllTanningMaterials(StaticFunctionTag*)
	{
		auto data = new CraftingData();
		return data->GetAllTanningMaterials();
	}

	VMResultArray<TESForm*> GetAllOreIngotMaterials(StaticFunctionTag*)
	{
		auto data = new CraftingData();
		return data->GetAllOreIngotMaterials();
	}

	VMResultArray<TESForm*> GetAllMaterialsFromConstructibleObject(StaticFunctionTag*)
	{
		auto data = new CraftingData();
		return data->GetAllMaterialsFromConstructibleObject();
	}


	void AddAllBaseEnchantmentsToList(StaticFunctionTag*, BGSListForm* result)
	{
		auto data = new CraftingData();
		return data->AddAllBaseEnchantmentsToList(result);
	}

	void AddAllCookingIngredientsToList(StaticFunctionTag*, BGSListForm* result)
	{
		auto data = new CraftingData();
		return data->AddAllCookingIngredientsToList(result);
	}

	void AddAllAlchemyIngredientsToList(StaticFunctionTag*, BGSListForm* result)
	{
		auto data = new CraftingData();
		return data->AddAllAlchemyIngredientsToList(result);
	}

	void AddAllSoulGemsToList(StaticFunctionTag*, BGSListForm* result, bool filledOnly)
	{
		auto data = new CraftingData();
		return data->AddAllSoulGemsToList(result, filledOnly);
	}

	void AddAllTanningMaterialsToList(StaticFunctionTag*, BGSListForm* result)
	{
		auto data = new CraftingData();
		return data->AddAllTanningMaterialsToList(result);
	}

	void AddAllOreIngotMaterialsToList(StaticFunctionTag*, BGSListForm* result)
	{
		auto data = new CraftingData();
		return data->AddAllOreIngotMaterialsToList(result);
	}

	void AddAllCraftingMaterialsToList(StaticFunctionTag*, BGSListForm* craftingList, bool addAlchemy, bool addCooking, bool addOreIngots, bool addSoulgems, bool addTanning, bool addFromCOBJ, BGSListForm* blacklist)
	{
		CraftingData* data;
		if (blacklist != NULL)
			data = new CraftingData(blacklist->forms);
		else
		{
			data = new CraftingData();
		}
		return data->AddAllCraftingMaterialsToList(craftingList, addAlchemy, addCooking, addOreIngots, addSoulgems, addTanning, addFromCOBJ);
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
			new NativeFunction8 <StaticFunctionTag, void, BGSListForm*, bool, bool, bool, bool, bool, bool, BGSListForm*>("AddAllCraftingMaterialsToList", "yeolde_papyrus", YeOldeGameData::AddAllCraftingMaterialsToList, registry));

		registry->RegisterFunction(
			new NativeFunction2 <StaticFunctionTag, void, BGSListForm*, BGSListForm*>("ConcatenateFormLists", "yeolde_papyrus", YeOldeGameData::ConcatenateFormLists, registry));

		return true;
	}
}
