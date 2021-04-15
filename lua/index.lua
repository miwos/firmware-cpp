-- Miwos.sendNoteOn(60, 127, 1)
-- Miwos.sendNoteOff(60, 127, 1)
collectgarbage('collect')
print(collectgarbage("count"))

-- local Timer = require('timer')

-- Miwos = {}

-- -- collectgarbage("collect")

-- function Miwos.update(time)
--   if (time % 1000 == 0) then
--     print(string.format('Memory usage: %dkb', collectgarbage('count')))
--   end
-- end