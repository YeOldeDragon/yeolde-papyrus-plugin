#pragma once

#include "skse64/GameTypes.h"
#include "skse64/GameForms.h"
#include "skse64/PapyrusArgs.h"



class CraftingData
{
private:
	VMResultArray<TESForm*> _baseEnchants;
	VMResultArray<TESForm*> _cookingIngredients;
	VMResultArray<TESForm*> _alchemyIngredients;
	VMResultArray<TESForm*> _soulGems;
	VMResultArray<TESForm*> _tanningMaterials;
	VMResultArray<TESForm*> _oreIngotMaterials;
	VMResultArray<TESForm*> _constructibleMaterials;
	tArray<TESForm*>		_blacklist;
	std::vector<FormType>   _badMaterialTypes;

	inline bool IsBadMaterial(TESForm* material)
	{
		return (std::find(_badMaterialTypes.begin(), _badMaterialTypes.end(), material->formType) != _badMaterialTypes.end());
	}

	bool IsBlackListed(TESForm* form)
	{
		for (int i = 0; i < _blacklist.count; i++)
		{
			if (_blacklist[i] == form)
			{
				_MESSAGE("IsBlackListed(): Item is blacklisted! (%u) %s", _blacklist[i]->formID, _blacklist[i]->GetName());
				return true;
			}
		}

		return false;
	}

	void Initialize();

public:
	CraftingData(tArray<TESForm*> blacklist)
	{
		_MESSAGE("CraftingData():Blacklist size: (%u)", blacklist.count);
		_blacklist = blacklist;
		Initialize();
	}

	CraftingData()
	{
		Initialize();
	}

	/// <summary>
	/// Get All the available base enchantments, from all active mods. 
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllBaseEnchantments();

	/// <summary>
	/// Get All the available cooking ingredients, from all active mods.
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllCookingIngredients();

	/// <summary>
	/// Get All the available ingredients, from all active mods.
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllAlchemyIngredients();

	/// <summary>
	/// Get All the available soul gems types.
	/// </summary>
	/// <param name="filledOnly">True if we only return filled soul gem type</param>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllSoulGems(bool filledOnly = true);

	/// <summary>
	/// Get All the available base enchantments, from all active mods. 
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllTanningMaterials();

	/// <summary>
	/// Get All the available tanning materials, from all active mods.
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllOreIngotMaterials();

	/// <summary>
	/// Get All the material from every Constructible Objects, from all active mods. 
	/// Somes items types are skipped: 
	/// -> FormType::kFormType_SoulGem
	/// -> FormType::kFormType_Potion
	/// -> FormType::kFormType_Weapon
	/// -> FormType::kFormType_Armor
	/// -> FormType::kFormType_Book
	/// -> FormType::kFormType_Ammo
	/// -> FormType::kFormType_LeveledItem
	/// -> FormType::kFormType_Outfit
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllMaterialsFromConstructibleObject();


	void AddAllBaseEnchantmentsToList(BGSListForm* result);
	void AddAllCookingIngredientsToList(BGSListForm* result);
	void AddAllAlchemyIngredientsToList(BGSListForm* result);
	void AddAllSoulGemsToList(BGSListForm* result, bool filledOnly);
	void AddAllTanningMaterialsToList(BGSListForm* result);
	void AddAllOreIngotMaterialsToList(BGSListForm* result);
	void AddAllCraftingMaterialsToList(BGSListForm* craftingList, bool addAlchemy, bool addCooking, bool addOreIngots, bool addSoulgems, bool addTanning, bool addFromCOBJ);

};