Modules = {}
require('timer')

-- Miwos = require('miwos')
-- Midi = require('midi')

Timer.schedule(Teensy.getTime() + 1000, function ()
  Log.info('oho!')
end)

-- Miwos.input:connect(1, Miwos.output, 1)

-- local bassPitch = require('BassPitch')()

-- Miwos.input:connect(1, bassPitch, 1)
-- bassPitch:connect(1, Miwos.output, 1)

-- hold:connect(1, chorder, 1)
-- chorder:connect(1, Miwos.output, 1)
-- arp:connect(1, Miwos.output, 1)

-- hold:connect(1, chorder, 1)
-- chorder:connect(1, Miwos.output, 1)

-- collectgarbage("collect")
-- info(collectgarbage('count'))