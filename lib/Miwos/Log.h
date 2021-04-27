#ifndef Log_h
#define Log_h

#include <LuaWrapper.h>
#include <MiwosBridge.h>

namespace Log {
  LuaWrapper *lua;
  MiwosBridge *bridge;

  namespace CppInterface {
    int warning(lua_State *L) {
      const char* text = lua_tostring(L, 1);
      bridge->warning(text);
      return 0;
    }

    int info(lua_State *L) {
      const char* text = lua_tostring(L, 1);
      bridge->info(text);
      return 0;    
    }
  }

  void begin(LuaWrapper *lua, MiwosBridge *bridge) {
    Log::lua = lua;
    Log::bridge = bridge;

    luaL_Reg library[] = {
      { "warning", CppInterface::warning },
      { "info", CppInterface::info },
      { NULL, NULL }
    };

    lua->registerLibrary("Log", library);
  }
}

#endif