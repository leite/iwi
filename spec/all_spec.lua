

describe('iwi', function ()

  local iwi, latitude, longitude

  teardown(function ()
    iwi, latitude, longitude, latitude_1, longitude_1 = nil, nil, nil, nil, nil
  end)

  setup(function ()
    iwi, latitude, longitude, latitude_1, longitude_1 = require('iwi'), 0.0, 0.0, 0.0, 0.0
  end)

  it('should be a table', function ()
    assert.is_table(iwi)
  end)

  it('check constants', function ()

    assert.is.equal(iwi.miles,      3963.1903)
    assert.is.equal(iwi.kilometers, 6378.1366)
    assert.is.equal(iwi.north,      0)
    assert.is.equal(iwi.east,       1)
    assert.is.equal(iwi.west,       2)
    assert.is.equal(iwi.south,      3)
  end)

  it('hash should be a string', function ()

    assert.equals(iwi.encode(55.7522222,  37.6155556,  10), 'ucftpvqu1z')
    assert.equals(iwi.encode(40.7141667,  -74.0063889, 9),  'dr5regy1n')
    assert.equals(iwi.encode(-23.8166667, -46.8,       8),  '6gy8x033')
    assert.equals(iwi.encode(-35.283333,  149.216667,  7),  'r3dp78b')
  end)

  it('should decode to latitude and latitude', function ()

    latitude, latitude_1, longitude, longitude_1 = iwi.decode('ucftpvqu1z')
    assert.equals(latitude,    55.752219557762145996)
    assert.equals(longitude,   37.615545988082885742)
    assert.equals(latitude_1,  55.752224922180175781)
    assert.equals(longitude_1, 37.615556716918945312)

    latitude, latitude_1, longitude, longitude_1 = iwi.decode('dr5regy1n')
    assert.equals(latitude,    40.714130401611328125)
    assert.equals(longitude,   -74.006395339965820312)
    assert.equals(latitude_1,  40.714173316955566406)
    assert.equals(longitude_1, -74.006352424621582031)

    latitude, latitude_1, longitude, longitude_1 = iwi.decode('6gy8x033')
    assert.equals(latitude,    -23.816814422607421875)
    assert.equals(longitude,   -46.80004119873046875)
    assert.equals(latitude_1,  -23.81664276123046875)
    assert.equals(longitude_1, -46.7996978759765625)

    latitude, latitude_1, longitude, longitude_1 = iwi.decode('r3dp78b')
    assert.equals(latitude,    -35.283966064453125)
    assert.equals(longitude,   149.21630859375)
    assert.equals(latitude_1,  -35.2825927734375)
    assert.equals(longitude_1, 149.217681884765625)
  end)

  it('hash should be valid', function ()

    assert.is_true(iwi.verify('sgu3fk87rshv6'))
    assert.is_true(iwi.verify('sv9hcc18k2'))
    assert.is_true(iwi.verify('sr2y7mem'))
    assert.is_true(iwi.verify('tue00eyc25t9'))
  end)

  it('adjacent hashes', function ()

    assert.equals(iwi.adjacent('sv9hcc18k2',    iwi.north), 'sv9hcc18k3')
    assert.equals(iwi.adjacent('sgu3fk87rshv6', iwi.east),  'sgu3fk87rshv7')
    assert.equals(iwi.adjacent('tue00eyc25t9',  iwi.west),  'tue00eyc25t3')
    assert.equals(iwi.adjacent('r3dp78b',       iwi.south), 'r3dp788')
  end)

  it('distance should match', function ()

    -- kilometers
    assert.equals(
      iwi.distance(55.7522222, 37.6155556, 40.7141667, -74.0063889, iwi.kilometers), 7518.7449109756489634
    )
    assert.equals(
      iwi.distance(-23.8166667, -46.8, 40.7141667, -74.0063889, iwi.kilometers), 7715.5748694448520837
    )
    assert.equals(
      iwi.distance(40.7141667, -74.0063889, 38.0291667, -78.4769444), 487.06290558245427746
    )
    assert.equals(
      iwi.distance(52.35, 4.9166667, 35.8997222, 14.5147222), 1981.2775519780357172
    )

    -- miles
    assert.equals(
      iwi.distance(52.35, 4.9166667, 55.7522222, 37.6155556, iwi.miles), 1335.5033589612942251
    )
    assert.equals(
      iwi.distance(35.8997222, 14.5147222, 55.7522222, 37.6155556, iwi.miles), 1750.6332184116220105
    )
    assert.equals(
      iwi.distance(-23.8166667, -46.8, -21.1666667, -47.8, iwi.miles), 194.12122053644137054
    )   
    assert.equals(
      iwi.distance(35.6961111111, 51.4230555556, 55.7522222, 37.6155556, iwi.miles), 1532.6551080902963804
    )
  end)

  it('destination should match', function ()

    latitude, longitude = iwi.destination(-21.1666667, -47.8, 35.0, 10.0)                 -- kilometers
    assert.equals(latitude,  -21.093072058860350637)
    assert.equals(longitude, -47.744774490291185032)

    latitude, longitude = iwi.destination(52.35, 4.9166667, 120, 20, iwi.kilometers)      -- kilometers
    assert.equals(latitude,  52.259895294594159054)
    assert.equals(longitude, 5.1708701618583399195)

    latitude, longitude = iwi.destination(38.0291667, -78.4769444, 73.33, 120, iwi.miles) -- miles
    assert.equals(latitude,  38.507773621446759194)
    assert.equals(longitude, -76.352977431921189577)

    latitude, longitude = iwi.destination(35.8997222, 14.5147222, 280, 200, iwi.miles)   -- miles
    assert.equals(latitude,  36.350080966116550485)
    assert.equals(longitude, 10.978551134139639345)
  end)

  it('neighbors should match', function ()

    assert.are.same(
      iwi.neighbors('6unp869p5r'), {
        north      = '6unp869p72', east       = '6unp869p5x',
        west       = '6unp869p5p', south      = '6unp869p5q',
        north_east = '6unp869p78', south_east = '6unp869p5w',
        north_west = '6unp869p70', south_west = '6unp869p5n'
      }
    )
    assert.are.same(
      iwi.neighbors('u173zfh0ye'), {
        north      = 'u173zfh0ys', east       = 'u173zfh0yg',
        west       = 'u173zfh0y7', south      = 'u173zfh0yd',
        north_east = 'u173zfh0yu', south_east = 'u173zfh0yf',
        north_west = 'u173zfh0yk', south_west = 'u173zfh0y6'
      }
    )
    assert.are.same(
      iwi.neighbors('sq6k4r3719'), {
        north      = 'sq6k4r371d', east       = 'sq6k4r371c',
        west       = 'sq6k4r3713', south      = 'sq6k4r3718',
        north_east = 'sq6k4r371f', south_east = 'sq6k4r371b',
        north_west = 'sq6k4r3716', south_west = 'sq6k4r3712'
      }
    )
    assert.are.same(
      iwi.neighbors('kzfsunym05'), {
        north      = 'kzfsunym0h', east       = 'kzfsunym07',
        west       = 'kzfsunyjpg', south      = 'kzfsunym04',
        north_east = 'kzfsunym0k', south_east = 'kzfsunym06',
        north_west = 'kzfsunyjpu', south_west = 'kzfsunyjpf'
      }
    )
  end)

end)