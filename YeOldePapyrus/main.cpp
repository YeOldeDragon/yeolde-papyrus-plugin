#include "common/IDebugLog.h"  // IDebugLog
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION
#include "skse64/PluginAPI.h"  // SKSEInterface, PluginInfo

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS
#include "version.h"  // VERSION_VERSTRING, VERSION_MAJOR

#include "YeOldeGameData.h"

static SKSEPapyrusInterface* g_papyrus = NULL;


extern "C" {
	__declspec(dllexport) bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\YeOldePlugin.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("YeOldePlugin v%s", MYFP_VERSION_VERSTRING);

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "YeOldePlugin";
		a_info->version = MYFP_VERSION_MAJOR;

		if (a_skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		} 
#if defined (SKYRIM_VR)
		else if (a_skse->runtimeVersion != RUNTIME_VR_VERSION_1_4_15) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}
#else
		else if (a_skse->runtimeVersion < RUNTIME_VERSION_1_5_80) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}
#endif	

		g_papyrus = (SKSEPapyrusInterface*)a_skse->QueryInterface(kInterface_Papyrus);

		if (!g_papyrus) {
			_MESSAGE("couldn't get papyrus interface");
			return false;
		}

		return true;
	}


	__declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		g_papyrus->Register(YeOldeGameData::RegisterFuncs);

		_MESSAGE("YeOldePlugin loaded");

		return true;
	}
};
