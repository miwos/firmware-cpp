#include "LuaWrapper.h"

void LuaWrapper::begin() {
  // Enable printf/sprintf to print floats for Teensy
  asm(".global _printf_float");
  L = luaL_newstate();
  luaL_openlibs(L);
}

bool LuaWrapper::check(int error) {
  if (error) {
    Serial.println(lua_tostring(L, -1));
    return false;
  } else {
    return true;
  }
}

bool LuaWrapper::execute(const char *string) {
  return check(luaL_dostring(L, string));
}

void LuaWrapper::registerFunction(const char *name, const lua_CFunction function) {
  lua_register(L, name, function);
}

// Compatability functions for serial output and SdFat file operations.
extern "C" {
  void lua_compat_print(const char *string) {
    Serial.print(string);
  }

  int lua_compat_fopen(const char *filename) {
    Serial.println("sd open");
    return 0;
    // return (lua_temp_stdio_file.fopen(filename, "r") ? 1 : 0);
  }

  void lua_compat_fclose() {
    Serial.println("sd close");
    //lua_temp_stdio_file.fclose();
  }

  int lua_compat_feof() {
    Serial.println("sd feof");
    return 0;
    //return lua_temp_stdio_file.feof();
  }

  size_t lua_compat_fread(void* ptr, size_t size, size_t count) {
    Serial.println("sd fread");
    return 0;
    //return lua_temp_stdio_file.fread(ptr, size, count);
  }

  int lua_compat_ferror() {
    Serial.println("-- sd ferror");
    return 0;
    //return lua_temp_stdio_file.ferror();
  }
}