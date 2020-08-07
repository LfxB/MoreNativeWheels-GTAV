/*
THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
http://dev-c.com
(C) Alexander Blade 2015
*/

#include "..\..\ScriptHookV_SDK\inc\main.h"
#include "script.h"
#include "keyboard.h"
#include "Util/Paths.h"
#include "Util/Logger.hpp"
#include "Util/Versions.h"


BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	std::string logFile = Paths::GetModuleFolder(hInstance) + modDir +
		"\\" + Paths::GetModuleNameWithoutExtension(hInstance) + ".log";
	logger.SetFile(logFile);
	Paths::SetOurModuleHandle(hInstance);
	Paths::SetModuleInfo(Paths::GetModuleFolder(hInstance), Paths::GetModuleName(hInstance));
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			scriptRegister(hInstance, ScriptMain);
			logger.Clear();
			logger.Write(INFO, "More Radio Wheels " + std::string(DISPLAY_VERSION));
			logger.Write(INFO, "Game version " + eGameVersionToString(getGameVersion()));
			logger.Write(INFO, "Script registered");
			break;
		case DLL_PROCESS_DETACH:
			unhide_all_stations();
			scriptUnregister(hInstance);
			break;
	}
	return TRUE;
}
