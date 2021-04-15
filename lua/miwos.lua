local Timer = require('timer')

local Miwos = {}

function Miwos:update(time)
  Timer:update(time)
end

function Miwos:sendNoteOn(note)
  sendNoteOff(unpack(note))
end

function Miwos:sendNoteOff(note)
  sendNoteOn(unpack(note))
end

function Miwos:sendNote(note, duration)
  self.sendNoteOn(note)
  Timer:schedule(getTime() + duration, function ()
    self.sendNoteOff(note)
  end)
end

function Miwos:handleNoteOn(note, velocity, channel)
  self:sendNoteOn({ note -12, velocity, channel })
  info(collectgarbage('count'))
end

function Miwos:handleNoteOff(note, velocity, channel)
  self:sendNoteOff({ note - 12, velocity, channel })
end

return Miwos