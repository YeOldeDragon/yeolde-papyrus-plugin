#pragma once

class VMClassRegistry;
struct StaticFunctionTag;

#include "skse64/GameTypes.h"
#include "skse64/GameForms.h"
#include "skse64/PapyrusArgs.h"

namespace YeOldeGameData
{
	/// <summary>
	/// Get All the available base enchantments, from all active mods. 
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllBaseEnchantments(StaticFunctionTag* functionTag = NULL);

	/// <summary>
	/// Get All the available cooking ingredients, from all active mods.
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllCookingIngredients(StaticFunctionTag* functionTag = NULL);

	/// <summary>
	/// Get All the available ingredients, from all active mods.
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllAlchemyIngredients(StaticFunctionTag* functionTag = NULL);

	/// <summary>
	/// Get All the available soul gems types.
	/// </summary>
	/// <param name="filledOnly">True if we only return filled soul gem type</param>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllSoulGems(StaticFunctionTag* functionTag = NULL, bool filledOnly = true);

	/// <summary>
	/// Get All the available base enchantments, from all active mods. 
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllTanningMaterials(StaticFunctionTag* functionTag = NULL);

	/// <summary>
	/// Get All the available tanning materials, from all active mods.
	/// </summary>
	/// <returns>An array of items.</returns>
	VMResultArray<TESForm*> GetAllOreIngotMaterials(StaticFunctionTag* functionTag = NULL);

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
	VMResultArray<TESForm*> GetAllMaterialsFromConstructibleObject(StaticFunctionTag* functionTag = NULL, bool addCookingIngredients = true);


	/// <summary>
	/// Register SKSE functions
	/// </summary>
	bool RegisterFuncs(VMClassRegistry* registry);
}