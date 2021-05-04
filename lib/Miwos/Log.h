#ifndef Log_h
#define Log_h

#include <LuaWrapper.h>
#include <MiwosBridge.h>

namespace Log {
  LuaWrapper *lua;
  MiwosBridge *bridge;

  namespace CppInterface {
    int logBegin(lua_State *L) {
      byte type = lua_tonumber(L, 1) - 1;
      bridge->logBegin(static_cast<MiwosBridge::LogType>(type));
      return 0;
    }

    int logEnd(lua_State *L) {
      bridge->logEnd();
      return 0;
    }
  }

  void begin(LuaWrapper *lua, MiwosBridge *bridge) {
    Log::lua = lua;
    Log::bridge = bridge;

    luaL_Reg library[] = {
      { "_begin", CppInterface::logBegin },
      { "_end", CppInterface::logEnd },
      { NULL, NULL }
    };

    lua->registerLibrary("Log", library);
    lua->execute(
      "function Log._log(type, ...)\n"
      "  Log._begin(type)\n"
      "  print(...)\n"
      "  Log._end()\n"
      "end\n"
      "function Log.info(...) Log._log(1, ...) end\n"
      "function Log.warning(...) Log._log(2, ...) end\n"
      "function Log.error(...) Log._log(3, ...) end\n"
    );
  }
}

#endif