#include "skse64/PapyrusArgs.h"
#include "skse64/GameForms.h"

namespace YeOldeFormList
{
	/// <summary>
	/// Add forms to a FormList
	/// </summary>
	/// <param name="list">FormList to add items in</param>
	/// <param name="forms">Forms items to add.</param>
	void AddForms(BGSListForm* list, VMResultArray<TESForm*> forms)
	{
		if (list)
		{
			for (std::size_t i = 0; i < forms.size(); i++)
			{
				TESForm* form = forms[i];
				if (form)
					CALL_MEMBER_FN(list, AddFormToList)(form);
			}
		}
	}
}