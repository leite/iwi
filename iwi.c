/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <xxleite@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <geohash.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <iwi.h>

// meta methods
const struct luaL_reg iwi_methods[] = { /*iwi_reg[] = {*/
  {"encode",    iwi_encode},
  {"decode",    iwi_decode},
  {"adjacent",  iwi_adjacent},
  {"distance",  iwi_distance},
  {"neighbors", iwi_neighbors},
  {NULL,        NULL}
};

const struct luaL_reg iwi_obj[] = {
  {"new", iwi_new},
  {NULL,  NULL}
};

// 
static int iwi_encode(lua_State *L) { 
  lua_pushnumber(L, 10);
  return 1;
}

// 
static int iwi_decode(lua_State *L) { return 0; }

// 
static int iwi_adjacent(lua_State *L) { return 0; }

// 
static int iwi_distance(lua_State *L) { return 0; }

// 
static int iwi_neighbors(lua_State *L) { return 0; }

// free garbage after gc call
static int iwi_free(lua_State *L) { 
  printf("## __gc\n");
  return 0; 
}

// new instance
static int iwi_new(lua_State *L) {

  lua_createtable(L, 0, 0);

  lua_pushnumber(L, GEOHASH_NORTH);
  lua_setfield(L, -2, "north");
  lua_pushnumber(L, GEOHASH_SOUTH);
  lua_setfield(L, -2, "south");
  lua_pushnumber(L, GEOHASH_WEST);
  lua_setfield(L, -2, "west");
  lua_pushnumber(L, GEOHASH_EAST);
  lua_setfield(L, -2, "east");
  lua_pushnumber(L, MILES);
  lua_setfield(L, -2, "miles");
  lua_pushnumber(L, KILOMETERS);
  lua_setfield(L, -2, "kilometers");

  luaL_newmetatable(L, "iwi");

  //lua_pushliteral(L, "__gc");
  lua_pushcfunction(L, iwi_free);
  lua_setfield(L, -2, "__gc");
  //lua_rawset(L, -3);

  luaL_newlib(L, iwi_methods);
  lua_setfield(L, -2, "__index");

  lua_setmetatable(L, -1);

  luaL_getmetatable(L, "iwi");
  lua_setmetatable(L, -2);
  return 1;  
}

// register lib
LUALIB_API int luaopen_iwi(lua_State *L) {
  luaL_register(L, "iwi", iwi_obj);
  return 1;
}

