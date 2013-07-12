local iwi = require('iwi')
local math = require('math')

assert(type(iwi)=='table', "fail, iwi should br a table")

local hash = iwi.encode(23.10, 45.467, 10)
assert(type(hash)=='string', "fail, hash should br a string")

local min_lat, max_lat, min_lon, max_long = iwi.decode(hash)

print(hash)
print(min_lat, max_lat, min_lon, max_long)
print(iwi.adjacent(hash, iwi.east))

print(iwi.distance(-23.864529, -46.789355, -23.883562, -46.793947))
print(iwi.distance(-23.722144, -46.691809, -22.947378, -48.430974))
print(iwi.distance(-23.722144, -46.691809, -23.722144, -46.691809))
local neighbors = iwi.neighbors(hash)

for k,v in pairs(neighbors) do
  print('  '.. k ..' = '.. v)
end

print(' >> "constants"')
for k,v in pairs(iwi) do
  if type(v)~='function' then
    print(k ..' = '.. tostring(v))
  end
end

print(' >> methods ')
for k,v in pairs(iwi) do
  if type(v)=='function' then
    print(k ..' = '.. tostring(v))
  end
end
