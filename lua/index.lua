-- Disable module cache
Modules = {}

local Timer = require('timer')
local Arp = require('arp')
effect = Arp()

function handleTime(time) Timer:update(time) end

function handleNoteOn(...) 
  if effect.handleNoteOn ~= nil then effect:handleNoteOn(...) end
end

function handleNoteOff(...) 
  if effect.handleNoteOff ~= nil then effect:handleNoteOff(...) end
end