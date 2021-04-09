#include "LuaWrapper.h"

StdioStream tempFile;

const char luaRequire[] = 
"function require(moduleName)\n"
"  if Modules == nil then\n"
"    Modules = {}\n"
"  end\n"
"  if Modules[moduleName] == nil then\n"
"    Modules[moduleName] = loadModule('lua/' .. moduleName .. '.lua')\n"
"  end\n"
"  return Modules[moduleName]\n"
"end\n";

int luaLoadModule(lua_State* L) {
  const char* fileName = lua_tostring(L, 1);
  luaL_loadfile(L, fileName);
  lua_call(L, 0, 1);
  return 1;
}

void LuaWrapper::begin() {
  // Enable printf/sprintf to print floats for Teensy.
  asm(".global _printf_float");
  L = luaL_newstate();
  luaL_openlibs(L);

  // Polyfill lua's `require` function.
  registerFunction("loadModule", luaLoadModule);
  execute(luaRequire);
}

bool LuaWrapper::check(int error) {
  if (error) {
    if (handleError != NULL) handleError(lua_tostring(L, -1));
    return false;
  } else {
    return true;
  }
}

bool LuaWrapper::execute(const char *string) {
  return check(luaL_dostring(L, string));
}

bool LuaWrapper::executeFile(const char *fileName) {
  return check(luaL_dofile(L, fileName));
}

void LuaWrapper::registerFunction(const char *name, const lua_CFunction function) {
  lua_register(L, name, function);
}

// Compatability functions for serial output and SD file operations.
extern "C" {
  void lua_compat_print(const char *string) {
    Serial.print(string);
  }

  int lua_compat_fopen(const char *fileName) {
    return tempFile.fopen(fileName, "r") ? 1 : 0;
  }

  void lua_compat_fclose() {
    tempFile.fclose();
  }

  int lua_compat_feof() {
    return tempFile.feof();
  }

  size_t lua_compat_fread(void* ptr, size_t size, size_t count) {
    return tempFile.fread(ptr, size, count);
  }

  int lua_compat_ferror() {
    return tempFile.ferror();
  }
}