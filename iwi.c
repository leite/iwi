/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <xxleite@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return
 * ----------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <math.h>
#include <geohash.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <iwi.h>

// meta methods
const struct luaL_reg iwi_methods[] = {
  {"encode",    iwi_encode},
  {"decode",    iwi_decode},
  {"adjacent",  iwi_adjacent},
  {"distance",  iwi_distance},
  {"neighbors", iwi_neighbors},
  {NULL,        NULL}
};

// 
static int iwi_encode(lua_State *L) {
  char *hash;
  double lat, lon;
  int len;

  lat  = (double)luaL_checknumber(L, 1);
  lon  = (double)luaL_checknumber(L, 2);
  len  = luaL_checknumber(L, 3);
  hash = GEOHASH_encode(lat, lon, len);

  lua_pushlstring(L, hash, sizeof(hash));
  
  free(hash);
  return 1;
}

// 
static int iwi_decode(lua_State *L) {
  GEOHASH_area *area;
  const char *hash;

  hash = luaL_checkstring(L, 1);
  area = GEOHASH_decode(hash);

  lua_pushnumber(L, area->latitude.min);
  lua_pushnumber(L, area->latitude.max);
  lua_pushnumber(L, area->longitude.min);
  lua_pushnumber(L, area->longitude.max);

  GEOHASH_free_area(area);
  return 4;
}

// 
static int iwi_adjacent(lua_State *L) { 
  const char *adj_hash;
  unsigned int direction;

  adj_hash  = luaL_checkstring(L, 1);
  direction = luaL_checknumber(L, 2);

  if(
      GEOHASH_NORTH!=direction && GEOHASH_SOUTH!=direction &&
      GEOHASH_EAST!=direction && GEOHASH_WEST!=direction
    ) {
    lua_pushstring(L, "invalid adjacent constant value");
    lua_error(L);
  } 

  adj_hash = GEOHASH_get_adjacent(adj_hash, direction);
  lua_pushlstring(L, adj_hash, sizeof(adj_hash));

  free((char*)adj_hash);
  return 1;
}

// 
static int iwi_neighbors(lua_State *L) { 
  GEOHASH_neighbors *neighbors;
  const char *hash;

  hash      = luaL_checkstring(L, 1);
  neighbors = GEOHASH_get_neighbors(hash);

  lua_newtable(L);
  
  lua_pushlstring(L, neighbors->north, sizeof(neighbors->north));
  lua_setfield(L, -2, "north");
  
  lua_pushlstring(L, neighbors->north_east, sizeof(neighbors->north_east));
  lua_setfield(L, -2, "north_east");
  
  lua_pushlstring(L, neighbors->north_west, sizeof(neighbors->north_west));
  lua_setfield(L, -2, "north_west");
  
  lua_pushlstring(L, neighbors->south, sizeof(neighbors->south));
  lua_setfield(L, -2, "south");
  
  lua_pushlstring(L, neighbors->south_east, sizeof(neighbors->south_east));
  lua_setfield(L, -2, "south_east");
  
  lua_pushlstring(L, neighbors->east, sizeof(neighbors->east));
  lua_setfield(L, -2, "east");

  lua_pushlstring(L, neighbors->west, sizeof(neighbors->west));
  lua_setfield(L, -2, "west");

  GEOHASH_free_neighbors(neighbors);
  return 1;
}

// spherical law of cosines, see http://bit.ly/16xdWlg
static int iwi_distance(lua_State *L) {
  double lat, lat2, lon, lon2, measure;

  lat  = (double)luaL_checknumber(L, 1);
  lon  = (double)luaL_checknumber(L, 2);
  lat2 = (double)luaL_checknumber(L, 3);
  lon2 = (double)luaL_checknumber(L, 4);

  if(lua_gettop(L)==5) {
    measure = (double)luaL_checknumber(L, 5);
    if(KILOMETERS!=measure && MILES!=measure) {
      lua_pushstring(L, "invalid measure constant value");
      lua_error(L);
    }
  } else {
    measure = KILOMETERS;
  }

  if(lat==lat2 && lon==lon2) {
    lua_pushnumber(L, 0);
    return 1;
  }

  lat  = (M_PI*lat)  / 180;
  lat2 = (M_PI*lat2) / 180;
  lon  = (M_PI*lon)  / 180;
  lon2 = (M_PI*lon2) / 180;

  lua_pushnumber(L, acos((sin(lat) * sin(lat2)) + (cos(lat) * cos(lat2) * cos(lon - lon2))) * measure);
  return 1;
}

// register lib
LUALIB_API int luaopen_iwi(lua_State *L) {
  luaL_register(L, "iwi", iwi_methods);

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

  return 1;
}