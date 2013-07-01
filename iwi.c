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

#define LUA_API __declspec(dllexport)
#define UINT unsigned int
#define lua_setConst(L, con, name) {lua_pushnumber(L, con); lua_setfield(L, -2, name);}

static UINT const MILES      = 33
static UINT const KILOMETERS = 34

//
static int iwi_encode(lua_State *L);
static int iwi_decode(lua_State *L);
static int iwi_adjacent(lua_State *L);
static int iwi_distance(lua_State *L);
static int iwi_neighbors(lua_State *L);

// meta methods
static const luaL_Reg iwi_reg[] = {
  {"encode",    iwi_encode},
  {"decode",    iwi_decode},
  {"adjacent",  iwi_adjacent},
  {"distance",  iwi_distance},
  {"neighbors", iwi_neighbors},
  {NULL, NULL}
};

// new instance
static int iwi_new(lua_State *L) {

  if(luaL_newmetatable(L, "iwi")) {
    
    lua_newtable(L);
    
    //
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
    
    //
    luaL_register(L, NULL, iwi_reg);
    lua_setfield(L, -2, "__index");
    
    //
    lua_pushcfunction(L, iwi_free);
    lua_setfield(L, -2, "__gc");
  }
  
  return 1;
}

// 
static int iwi_encode(lua_State *L) {
  return 1;
}

// 
static int iwi_decode(lua_State *L) {
  return 1;
}

// 
static int iwi_adjacent(lua_State *L) {
  return 1;
}

// 
static int iwi_distance(lua_State *L) {
  return 1;
}

// 
static int iwi_neighbors(lua_State *L) {
  return 1;
}

// free garbage after gc call
static int iwi_free(lua_State *L) {
  // ...
}
