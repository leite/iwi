/**
 *
 * License: https://www.gnu.org/licenses/lgpl.html LGPL version 3
 * Authors: leite <xico@simbio.se> and Steve Salevan <steve.salevan@gmail.com>
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <geohash.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifndef LUA_API
#define LUA_API __declspec(dllexport)
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#if LUA_VERSION_NUM > 501
#define luaL_register(L,n,l)  (luaL_newlib(L,l))
#endif

#define lua_set_const(L, con, name) {lua_pushnumber(L, con); lua_setfield(L, -2, name);}
#define lua_set_sconst(L, con, name) {lua_pushstring(L, con); lua_setfield(L, -2, name);}

static double const MILES      = 3963.1903;
static double const KILOMETERS = 6378.1366;


//
static int iwi_verify(lua_State *L) {
  const char *hash = luaL_checkstring(L, 1);
  lua_pushboolean(L, GEOHASH_verify_hash(hash));
  return 1;
}

// 
static int iwi_encode(lua_State *L) {
  double lat  = (double)luaL_checknumber(L, 1);
  double lon  = (double)luaL_checknumber(L, 2);
  int    len  = luaL_checknumber(L, 3);
  char  *hash = GEOHASH_encode(lat, lon, len);

  lua_pushlstring(L, hash, strlen(hash));

  free(hash);
  return 1;
}

#ifdef DEBUG

static unsigned long long const BITMASK = 0xFFFFFFFFFFFFFFFF;

static int iwi_encode_double(lua_State *L) {
  double lat  = (double)luaL_checknumber(L, 1);
  double lon  = (double)luaL_checknumber(L, 2);
  int    len  = luaL_checknumber(L, 3);

  lua_pushnumber(L, GEOHASH_encode_double(lat, lon, len));
  return 1;
}

static int iwi_get_bbox_range(lua_State *L) {
  double geohash   = (double)luaL_checknumber(L, 1);
  int    range     = luaL_checknumber(L, 2);
  unsigned long long rangeLow  = (unsigned long long)geohash;
  unsigned long long rangeHigh = (unsigned long long)geohash;

  rangeHigh |= (((uint64_t) 0x1 << (range)) - 1);
  rangeLow &= BITMASK << range;

  lua_pushnumber(L, (double)rangeLow);
  lua_pushnumber(L, (double)rangeHigh);
  return 2;
}

#endif

// 
static int iwi_decode(lua_State *L) {

  const char   *hash = luaL_checkstring(L, 1);
  GEOHASH_area *area = GEOHASH_decode(hash);

  lua_pushnumber(L, area->latitude.min);
  lua_pushnumber(L, area->latitude.max);
  lua_pushnumber(L, area->longitude.min);
  lua_pushnumber(L, area->longitude.max);

  GEOHASH_free_area(area);
  return 4;
}

// 
static int iwi_adjacent(lua_State *L) { 

  char *adj_hash         = (char*)luaL_checkstring(L, 1);
  unsigned int direction = luaL_checknumber(L, 2);

  switch(direction) {
    case GEOHASH_NORTH:
    case GEOHASH_SOUTH:
    case GEOHASH_EAST:
    case GEOHASH_WEST:
      adj_hash = GEOHASH_get_adjacent(adj_hash, direction);
      lua_pushlstring(L, adj_hash, strlen(adj_hash));
      break;
    default:
      return luaL_error(L, "invalid adjacent constant value");
  }

  free(adj_hash);
  return 1;
}

// 
static int iwi_neighbors(lua_State *L) { 

  const char *hash             = luaL_checkstring(L, 1);
  GEOHASH_neighbors *neighbors = GEOHASH_get_neighbors(hash);

  lua_newtable(L);

  lua_set_sconst(L, neighbors->north,      "north");
  lua_set_sconst(L, neighbors->north_east, "north_east");
  lua_set_sconst(L, neighbors->north_west, "north_west");
  lua_set_sconst(L, neighbors->south,      "south");
  lua_set_sconst(L, neighbors->south_east, "south_east");
  lua_set_sconst(L, neighbors->south_west, "south_west");
  lua_set_sconst(L, neighbors->east,       "east");
  lua_set_sconst(L, neighbors->west,       "west");

  GEOHASH_free_neighbors(neighbors);
  return 1;
}

// spherical law of cosines, see http://bit.ly/16xdWlg
static int iwi_distance(lua_State *L) {
  double measure;

  double lat  = (double)luaL_checknumber(L, 1);
  double lon  = (double)luaL_checknumber(L, 2);
  double lat2 = (double)luaL_checknumber(L, 3);
  double lon2 = (double)luaL_checknumber(L, 4);

  if(lua_gettop(L)==5) {
    measure = (double)luaL_checknumber(L, 5);
    if(KILOMETERS!=measure && MILES!=measure)
      return luaL_error(L, "invalid measure constant value");
  } else {
    measure = KILOMETERS;
  }

  if(lat==lat2 && lon==lon2) {
    lua_pushnumber(L, 0);
    return 1;
  }

  lat  = (M_PI*lat)  / 180.0;
  lat2 = (M_PI*lat2) / 180.0;
  lon  = (M_PI*lon)  / 180.0;
  lon2 = (M_PI*lon2) / 180.0;

  lua_pushnumber(L, acos((sin(lat) * sin(lat2)) + (cos(lat) * cos(lat2) * cos(lon - lon2))) * measure);
  return 1;
}

// 
static int iwi_destination(lua_State *L) {
  double lat2, lon2, measure;

  double lat     = (double)luaL_checknumber(L, 1);
  double lon     = (double)luaL_checknumber(L, 2);
  double bearing = (double)luaL_checknumber(L, 3);
  double dist    = (double)luaL_checknumber(L, 4);

  if(lua_gettop(L)==5) {
    measure = (double)luaL_checknumber(L, 5);
    if(KILOMETERS!=measure && MILES!=measure)
      return luaL_error(L, "invalid measure constant value");
  } else {
    measure = KILOMETERS;
  }

  lat     = (M_PI*lat)     / 180.0;
  lon     = (M_PI*lon)     / 180.0;
  bearing = (M_PI*bearing) / 180.0;
  dist    = dist / measure;

  lat2 = asin(((sin(lat) * cos(dist)) + (cos(lat) * sin(dist) * cos(bearing))));
  lon2 = lon + atan2((sin(bearing) * sin(dist) * cos(lat)), (cos(dist) - (sin(lat) * sin(lat2))));

  lua_pushnumber(L, ((lat2 * 180) / M_PI));
  lua_pushnumber(L, (((fmod((lon2 + (3 * M_PI)), (2 * M_PI)) - M_PI) * 180) / M_PI));
  return 2;
}

// meta methods
const struct luaL_Reg iwi_methods[] = {
#ifdef DEBUG
  {"encode_double",  &iwi_encode_double},
  {"get_bbox",       &iwi_get_bbox_range},
#endif
  {"encode",         &iwi_encode},
  {"decode",         &iwi_decode},
  {"adjacent",       &iwi_adjacent},
  {"distance",       &iwi_distance},
  {"destination",    &iwi_destination},
  {"neighbors",      &iwi_neighbors},
  {"verify",         &iwi_verify},
  {NULL,             NULL}
};

// register lib
LUALIB_API int luaopen_iwi(lua_State *L) {

  lua_newtable(L);
  luaL_register(L, NULL, iwi_methods);

  lua_set_const(L, GEOHASH_NORTH, "north");
  lua_set_const(L, GEOHASH_SOUTH, "south");
  lua_set_const(L, GEOHASH_WEST,  "west");
  lua_set_const(L, GEOHASH_EAST,  "east");
  lua_set_const(L, MILES,         "miles");
  lua_set_const(L, KILOMETERS,    "kilometers");

  return 1;
}
