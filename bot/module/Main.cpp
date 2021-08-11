#ifdef WIN32
#define MTAEXPORT extern "C" __declspec(dllexport)
#else
#define MTAEXPORT extern "C"
#endif

#include "include/ILuaModuleManager.h"

#include "BotSession.h"

#ifndef WIN32
	#include "luaimports/luaimports.linux.h"
#endif

ILuaModuleManager10* pModuleManager = nullptr;

// Initialization function (module entrypoint)
MTAEXPORT bool InitModule(ILuaModuleManager10* pManager, char* szModuleName, char* szAuthor, float* fVersion)
{
#ifndef WIN32
	ImportLua();
#endif

    pModuleManager = pManager;

	// Set the module info
    const auto module_name	= "Discord Module";
    const auto author		= "DizzasTeR";
	std::memcpy(szModuleName, module_name,	MAX_INFO_LENGTH);
	std::memcpy(szAuthor,     author,		MAX_INFO_LENGTH);
	*fVersion = 1.0f;

    return true;
}

MTAEXPORT void RegisterFunctions(lua_State* lua_vm)
{
	if (!pModuleManager || !lua_vm)
		return;

	BotSession::Init(lua_vm);
}

MTAEXPORT bool DoPulse()
{
	BotSession::Pulse();
	return true;
}

MTAEXPORT bool ShutdownModule()
{
    return true;
}

MTAEXPORT bool ResourceStopping(lua_State* luaVM)
{
	return true;
}

MTAEXPORT bool ResourceStopped(lua_State* luaVM)
{
	return true;
}
