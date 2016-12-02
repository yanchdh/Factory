#pragma once
#include <lua.hpp>
#include <string>

extern "C" { int luaopen_pb(lua_State *L); }

using std::string;

const string rule_lua_path = "E:\\Factory\\share\\rule_lua\\";