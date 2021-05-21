#ifndef Log_h
#define Log_h

#include <LuaWrapper.h>
#include <MiwosBridge.h>

namespace Log {
  LuaWrapper *lua;
  MiwosBridge *bridge;

  namespace CppInterface {
    int dump(lua_State *L) {
      const char* text = lua_tostring(L, 1);
      bridge->dump(text);
      return 0;
    }

    int error(lua_State *L) {
      const char* text = lua_tostring(L, 1);
      bridge->error(text);
      return 0;
    }

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

    int flush(lua_State *L) {
      bridge->flush();
      return 0;
    }
  }

  void begin(LuaWrapper *lua, MiwosBridge *bridge) {
    Log::lua = lua;
    Log::bridge = bridge;

    luaL_Reg library[] = {
      { "error", CppInterface::error },
      { "warning", CppInterface::warning },
      { "info", CppInterface::info },
      { "dump", CppInterface::dump },
      { "flush", CppInterface::dump },
      { NULL, NULL }
    };

    lua->registerLibrary("Log", library);
  }
}

#endif