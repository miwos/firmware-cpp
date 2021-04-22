local BassPitch = Miwos.Module()

function BassPitch:init()
  self.control = 31
end


function BassPitch:input1_noteOn(message)
  local note = unpack(message.payload)
  self:output(1, Midi.ControlChange(self.control, note, 2))
end

return BassPitch