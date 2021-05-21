#include "LuaWrapper.h"

StdioStream tempFile;

const char luaGlobalVariables[] =
"_LOADED = {}\n"
"LuaRoot = 'lua/'\n";

const char luaRequire[] = 
"function require(moduleName)\n"
"  if _LOADED[moduleName] == nil then\n"
"    local root = LuaRoot or 'lua/'\n"
"    local path = string.gsub(moduleName, '%.', '/')\n"
"    _LOADED[moduleName] = loadfile(LuaRoot .. path .. '.lua')\n"
"  end\n"
"  return _LOADED[moduleName]\n"
"end\n";

LuaWrapper::LuaWrapper(Stream* serial) {
  this->serial = serial;
}

int LuaWrapper::luaLoadFile() {
  const char* fileName = lua_tostring(L, 1);
  check(luaL_loadfile(L, fileName));
  lua_call(L, 0, 1);
  return 1;
}

void LuaWrapper::errorBegin() {
  if (handleErrorBegin != NULL) handleErrorBegin();
}

void LuaWrapper::errorEnd() {
  if (handleErrorEnd != NULL) handleErrorEnd();
}

bool LuaWrapper::check(int luaHasError) {
  if (luaHasError) {
    errorBegin();
    serial->print(lua_tostring(L, -1));
    errorEnd();
    lua_pop(L, 1);
  }
  return luaHasError ? false : true;
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

void LuaWrapper::registerLibrary(const char *name, const luaL_reg library[]) {
  luaL_register_light(L, name, library);
}

bool LuaWrapper::getFunction(const char *name, bool logError) {
  lua_getglobal(L, name);

  if (!lua_isfunction(L, -1)) {
    if (logError) {
      errorBegin();
      serial->printf(F("Can't find function `%s`."), name);
      errorEnd();
    }
    lua_pop(L, 1);
    return false;
  }

  return true;  
}

bool LuaWrapper::getFunction(const char *name) {
  return getFunction(name, true);
}

bool LuaWrapper::getFunction(const char *table, const char *name, bool logError) {
  lua_getglobal(L, table);
  if (!lua_istable(L, -1)) {
    if (logError) {
      errorBegin();
      serial->printf(F("Can't find table `%s`."), table);
      errorEnd();
    }
    lua_pop(L, 1);
    return false;
  }

  lua_getfield(L, -1, name);
  // Important: remove table from stack again.
  lua_remove(L, -2);

  if (!lua_isfunction(L, -1)) {
    if (logError) {
      errorBegin();
      serial->printf(F("Can't find function `%s` in table `%s`."), name, table);
      errorEnd();
    }
    lua_pop(L, 1);
    return false;
  }

  return true;
}

bool LuaWrapper::getFunction(const char *table, const char *name) {
  return getFunction(table, name, true);
}

void LuaWrapper::push(lua_Number number) { lua_pushnumber(L, number); }

void LuaWrapper::push(const char *string) { lua_pushstring(L, string); }

void LuaWrapper::call(byte argsCount = 0, byte resultsCount = 0) {
  check(lua_pcall(L, argsCount, resultsCount, 0));
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
  registerFunction("loadfile", [](lua_State* L) {
    return that->luaLoadFile();
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