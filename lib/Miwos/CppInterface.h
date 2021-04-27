#ifndef CppInterface_h
#define CppInterface_h

#include <LuaWrapper.h>
#include <MiwosBridge.h>
#include "Displays.h"

namespace CppInterface {
  LuaWrapper *lua;
  MiwosBridge *bridge;

  namespace Display {
    int write(lua_State *L) {
      /*byte index = */lua_tonumber(L, 1);
      const char* text = lua_tostring(L, 2);

      Displays::display.clearDisplay();
      Displays::display.write(text);
      Displays::display.display();
      
      return 0;
    }
  }

  namespace Teensy {


    int getTime(lua_State *L) {
      lua->push(millis());
      return 1;
    }
  }

  namespace Log {
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
    CppInterface::lua = lua;
    CppInterface::bridge = bridge;

    const luaL_reg displayLibrary[] = {
      { "write", Display::write },
      { NULL, NULL }
    };

    const luaL_reg logLibrary[] = {
      { "warning", Log::warning },
      { "info", Log::info },
      { NULL, NULL }
    };    

    lua->registerLibrary("Display", displayLibrary); 
    lua->registerLibrary("Log", logLibrary); 
  }
};

#endif