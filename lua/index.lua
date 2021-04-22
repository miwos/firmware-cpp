Modules = {}
Miwos = require('miwos')
Midi = require('midi')

Miwos.output:input(1, Midi.NoteOn(60, 127, 1))

Miwos.Timer:schedule(getTime() + 1000, function ()
  Miwos.output:input(1, Midi.NoteOff(60, 127, 1))
end)

info(collectgarbage('count'))



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