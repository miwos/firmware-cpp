local class = require('class')
local Module = require('module')
local Midi = require('midi')

local Miwos = class(Module)

function Miwos.Module()
  return class(Module)
end

function Miwos.update()

end

function Miwos.handleNoteOn(...)
  self:output(1, Midi.NoteOn(...))
end

function Miwos.handleNoteOff(...)
  self:output(1, Midi.NoteOff(...))
end

return Miwos

-- local Timer = require('timer')

-- local Miwos = {}

-- function Miwos:update(time)
--   -- Timer:update(time)
-- end

-- function Miwos:sendNote(note, duration)
--   self.sendNoteOn(unpack(note))
--   Timer:schedule(getTime() + duration, function ()
--     self.sendNoteOff(note)
--   end)
-- end

-- function Miwos:handleNoteOn(note, velocity, channel)
--   self:sendNoteOn({ note, velocity, channel })
-- end

-- function Miwos:handleNoteOff(note, velocity, channel)
--   self:sendNoteOff({ note, velocity, channel })
-- end