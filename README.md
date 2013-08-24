# summary

iwi is a simple [Lua](http://www.lua.org/) [geohash](http://en.wikipedia.org/wiki/Geohash) binding for [libgeohash](https://github.com/lyokato/libgeohash).

# WTF iwi means?

iwi is a [nheengatu](http://en.wikipedia.org/wiki/Nheengatu_language) word that means "land" or even "earth" in some concepts.

# help and support

please fill an issue or help it doing a clone and then a pull request.

# license

[BEER-WARE](http://en.wikipedia.org/wiki/Beerware), see source

# prerequisites

+ [libgeohash](https://github.com/lyokato/libgeohash) (of course) 
+ [Lua](http://www.lua.org/) ([Luajit](http://luajit.org/))
+ [gcc](http://gcc.gnu.org/) and [make](http://www.gnu.org/software/make/) ([min-gw32](http://sourceforge.net/projects/mingw/files/MinGW/) on windows)
+ [upx](http://upx.sourceforge.net/) (optional)

# installation

compatible with *NIX systens, supposed to works on windows. You only need to edit some vars on makefile, for basic instalation use:

    # make

if you have upx use this for better output.
  
    # make release
  
for cleanup

    # make clean
  
# basic usage

```lua

local gh = require('iwi')

-- directions
--[[
gh.west
gh.east
gh.north
gh.south
--]]
-- units
--[[
gh.miles
gh.kilometers
]]

-- encodes latitude, and longitude with length
-- returns geohash
local geohash = gh.encode(23.11, 47.999, 5)

-- decodes geohash
-- returns min_latiture, max_latitude, min_longitude and max_longitude 
local min_lat, max_lat, min_lon, max_lon = gh.decode("abY9x")

-- get adjacent hash by origin's hash and direction
-- returns adjacent hash
local adj_geohash = gh.adjacent("abY9x", gh.west) -- or .east, .north, .south

-- get neighbors' hash by origin's hash
-- return a table with north, north_east, north_west, 
--   south, south_east, south_west, east and west neighbors
local neighbors = gh.neighbors("abY9x")

-- this is an optional feature, i think it might be useful
-- get distance between two pairs of longitude and latitude with optional unit,
--   defaults to kilometers
local distance = gh.distance(lat, lon, lat2, lon2, gh.kilometers) -- or gh.miles or nothing

``` 

# tests

see test.lua ...

# TODO

+ support luvit module style
+ create a test suite
+ improve makefile

% June 30th, 2013 -03 GMT
