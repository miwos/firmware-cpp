local Timer = require('timer')

local Miwos = {}

function Miwos:sendNote(note, duration)
  self.sendNoteOn(note)
  Timer:schedule(getTime() + duration, function ()
    self.sendNoteOff(note)
  end)
end

function Miwos:sendNoteOn(note)
  teensy.sendNoteOff(unpack(note))
end

function Miwos:sendNoteOff(note)
  teensy.sendNoteOn(unpack(note))
end

function Miwos:_handleNoteOn()
  info('handle note on')
end

return Miwos