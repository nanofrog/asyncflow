#include <iostream>
#include <string>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lua/export_lua.h"


int main(int argc, char** argv)
{
    // Create a new Lua state
    lua_State* L = luaL_newstate();
    if (argc < 2)
        std::cout << "Usage: " << argv[0] << " <script.lua>" << std::endl;

    std::string file_name = std::string(argv[1]);
    std::cout << "running " << file_name << std::endl;

    // Load the Lua libraries
    luaL_openlibs(L);
    luaopen_asyncflow(L);

    // Load and run the Lua script
    if (luaL_dofile(L, file_name.c_str()) != LUA_OK) {
        // If there is an error, print it
        printf("error %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from stack
    }

    // Close the Lua state
    lua_close(L);

    return 0;
}