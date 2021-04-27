Modules = {}

require('timer')
require('encoder')
require('midi')
require('miwos')

local Chorder = require('chorder')
local chorder = Chorder()

local Effect = require('arp')
effect = Effect()

Miwos.input:connect(1, chorder, 1)
chorder:connect(1, effect, 1)
effect:connect(1, Miwos.output, 1)



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