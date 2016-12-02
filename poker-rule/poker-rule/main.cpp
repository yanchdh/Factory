#include "classicddz.h"
#include <iostream>

int main(int args, char* argv[])
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_pb(L);
	if (luaL_loadfile(L, "classicddz.lua"))
	{
		fprintf(stderr, "luaL_loadfile error, %s\n", lua_tostring(L, -1));
	}
	if (lua_pcall(L, 0, -1, 0))
	{
		fprintf(stderr, "lua_pcall error, %s\n", lua_tostring(L, -1));
	}
	system("pause");
	return 0;
}