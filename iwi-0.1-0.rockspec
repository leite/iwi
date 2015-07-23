package = "iwi"
version = "0.1-0"

description = {
  summary  = "Lua binding to libgeohash",
  detailed = "Simple Lua geohash methods on top of libgeohash",
  homepage = "http://rocks.simbio.se/iwi",
  license  = "LGPL v3"
}

source = {
  url    = "git://github.com/leite/iwi.git",
  branch = "v0.1"
}

dependencies = {"lua >= 5.1, < 5.3"}

external_dependencies = {
  platforms = {
    unix = {
      LIBGEOHASH = {header="geohash.h", library="geohash"}
    }
  }
}

build = {
  type    = "builtin",
  modules = {
    iwi = {
      sources   = {"iwi.c"},
      libraries = {"m", "geohash"},
      incdirs   = {"$(LIBGEOHASH_INCDIR)"},
      libdirs   = {"$(LIBGEOHASH_LIBDIR)"}
    }
  }
}