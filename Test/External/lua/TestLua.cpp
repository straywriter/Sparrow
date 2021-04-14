// luaexample.cpp
extern "C"
{
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

}
#include <string.h>

int main(int argc, char *argv[])
{
    // initialization
    lua_State *L = lua_open();
    luaL_openlibs(L);

    // execute script
    const char lua_script[] = "print('Hello World!')";
    int load_stat = luaL_loadbuffer(L, lua_script, strlen(lua_script), lua_script);
    lua_pcall(L, 0, 0, 0);

    // cleanup
    lua_close(L);
    return 0;
}