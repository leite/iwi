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

#ifndef UINT
#define UINT unsigned int
#endif

#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
#define lua_rawlen lua_objlen
#define lua_getuservalue lua_getfenv
#define lua_setuservalue lua_setfenv
#define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))

static void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup, "too many upvalues");
  for (; l->name != NULL; l++) {  /* fill the table with given functions */
    int i;
    for (i = 0; i < nup; i++)  /* copy upvalues to the top */
      lua_pushvalue(L, -nup);
    lua_pushstring(L, l->name);
    lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
    lua_settable(L, -(nup + 3));
  }
  lua_pop(L, nup);  /* remove upvalues */
}
#endif

static UINT const MILES      = 33;
static UINT const KILOMETERS = 66;

//
static int iwi_encode(lua_State *L);
static int iwi_decode(lua_State *L);
static int iwi_adjacent(lua_State *L);
static int iwi_distance(lua_State *L);
static int iwi_neighbors(lua_State *L);
static int iwi_free(lua_State *L);
static int iwi_new(lua_State *L);