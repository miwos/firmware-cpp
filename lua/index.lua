Modules = {}
Miwos = require('miwos')
Midi = require('midi')

local Hold = require('hold')
local hold = Hold()

local chorder = require('chorder')()

Miwos.input:connect(1, hold, 1)
hold:connect(1, chorder, 1)
chorder:connect(1, Miwos.output, 1)

-- hold:connect(1, chorder, 1)
-- chorder:connect(1, Miwos.output, 1)
-- arp:connect(1, Miwos.output, 1)

-- hold:connect(1, chorder, 1)
-- chorder:connect(1, Miwos.output, 1)

-- collectgarbage("collect")
-- info(collectgarbage('count'))