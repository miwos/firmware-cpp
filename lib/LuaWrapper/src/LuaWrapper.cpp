#include "LuaWrapper.h"

StdioStream tempFile;

const char luaGlobalVariables[] =
"Modules = {}\n"
"LuaRoot = 'lua/'\n"
"Debug = false\n"
"function test ()\n print('TEST')\n end\n";

const char luaRequire[] = 
"function require(moduleName)\n"
"  local isCached = Modules[moduleName] ~= nil\n"
"  if not isCached then\n"
"    Modules[moduleName] = loadModule(LuaRoot .. moduleName .. '.lua')\n"
"  end\n"
"  if Debug then\n"
"    print(string.format('Require module `%s` from %s', moduleName, isCached and 'cache' or 'file'))\n"
"  end\n"
"  return Modules[moduleName]\n"
"end\n";

int LuaWrapper::luaLoadModule() {
  const char* fileName = lua_tostring(L, 1);
  check(luaL_loadfile(L, fileName));
  lua_call(L, 0, 1);
  return 1;
}

bool LuaWrapper::check(int luaHasError) {
  if (luaHasError) {
    error(lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  return luaHasError ? false : true;
}

void LuaWrapper::error(const char *text) {
  if (handleError != NULL) handleError(text);
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

bool LuaWrapper::getFunction(const char *name) {
  lua_getglobal(L, name);
  bool isFunction = lua_isfunction(L, -1);

  // TODO add function name to error message.
  if (!isFunction) error("Can't find global function.");

  return isFunction;
}

void LuaWrapper::push(lua_Number number) { lua_pushnumber(L, number); }

void LuaWrapper::push(const char *string) { lua_pushstring(L, string); }

void LuaWrapper::call(byte argsCount = 0, byte resultsCount = 0) {
  check(lua_pcall(L, argsCount, resultsCount, 0));
  // if (lua_pcall(L, argsCount, resultsCount, 0) != 0) {
  //   error("error running function");
  // }

  // lua_call(L, argsCount, resultsCount);
}


void LuaWrapper::begin() {
  // Enable printf/sprintf to print floats for Teensy.
  asm(".global _printf_float");
  L = luaL_newstate();
  luaL_openlibs(L);

  // Add global variables.
  execute(luaGlobalVariables);

  // Polyfill lua's `require` function.
  static LuaWrapper *that = this;
  registerFunction("loadModule", [](lua_State* L) {
    return that->luaLoadModule();
  });
  execute(luaRequire);
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