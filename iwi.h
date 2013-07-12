/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <xxleite@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return
 * ----------------------------------------------------------------------------
 */

#ifndef LUA_API
#define LUA_API __declspec(dllexport)
#endif

static double const MILES      = 3963.1903;
static double const KILOMETERS = 6378.1366;

//
static int iwi_encode(lua_State *L);
static int iwi_decode(lua_State *L);
static int iwi_adjacent(lua_State *L);
static int iwi_distance(lua_State *L);
static int iwi_neighbors(lua_State *L);
static int iwi_free(lua_State *L);
static int iwi_new(lua_State *L);