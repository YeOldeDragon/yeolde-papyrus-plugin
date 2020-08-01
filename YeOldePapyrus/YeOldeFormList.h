#pragma once


#include "skse64/PapyrusArgs.h"
#include "skse64/GameForms.h"


namespace YeOldeFormList
{
	/// <summary>
	/// Add forms to a FormList
	/// </summary>
	/// <param name="list">FormList to add items in</param>
	/// <param name="forms">Forms items to add.</param>
	void AddForms(BGSListForm* list, VMResultArray<TESForm*>* forms);


	void ConcatenateLists(BGSListForm* list, BGSListForm* listToAdd);
}