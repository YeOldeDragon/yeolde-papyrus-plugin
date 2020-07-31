#pragma once

#include "skse64/PapyrusArgs.h"
#include "skse64/GameForms.h"

namespace YeOldeForm
{
	/// <summary>
	/// Return true if the TESForm has the specified Keyword
	/// </summary>
	/// <param name="thisForm">The form to check for</param>
	/// <param name="keywordStr">The keyword (string)</param>
	/// <returns>true if the TESForm has the keyword. false if not.</returns>
	bool HasKeywords(TESForm* thisForm, std::string keywordStr);
}