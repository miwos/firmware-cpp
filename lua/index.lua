Modules = {}

require('timer')
require('encoder')
require('midi')
require('miwos')

-- collectgarbage("collect")
-- Log.info(collectgarbage('count'))

-- Miwos = require('miwos')
-- Midi = require('midi')

-- Timer:cancel(id)

Miwos.input:connect(1, Miwos.output, 1)

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