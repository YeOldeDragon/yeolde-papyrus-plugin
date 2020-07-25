#include "skse64/PapyrusArgs.h"
#include "skse64/GameForms.h"

#include "skse64/GameRTTI.h"


namespace YeOldeForm
{
	bool HasKeywords(TESForm* thisForm, std::string keywordStr)
	{
		if (!thisForm)
			return false;

		BGSKeywordForm* pKeywords = DYNAMIC_CAST(thisForm, TESForm, BGSKeywordForm);

		if (pKeywords)
		{
			BGSKeyword* pKeyword = NULL;

			for (UInt32 n = 0; n < pKeywords->numKeywords; n++) {
				pKeyword = pKeywords->keywords[n];

				if (pKeyword)
				{
					std::string s = std::string(pKeyword->keyword.Get());
					//_MESSAGE("HasKeywords() -> %s", s.c_str());

					if (!s.compare(keywordStr))
						return true;
				}
			}
		}

		return false;
	}
}