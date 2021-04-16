local Chorder = Miwos.Module()

function Chorder:init()
  self.notes = {}
  self.inputs = 1
  self.outputs = 1
end

function Chorder:input1(message)
  if (message.type == Midi.TypeNoteOn) then
    self:output(1, message)
  end
end


-- function Chorder:handleNoteOn(note, velocity, channel)
--   sendNoteOn(note, velocity, channel)
--   self:sendChordNote(note + 3, velocity, channel)
--   self:sendChordNote(note + 7, velocity, channel)
-- end

-- function Chorder:handleNoteOff(...)
--   sendNoteOff(...)
--   self:clear()
-- end

-- function Chorder:sendChordNote(...)
--   table.insert(self.notes, {...})
--   sendNoteOn(...)
-- end

-- function Chorder:clear()
--   for i=1, #self.notes do
--     sendNoteOff(unpack(self.notes[i]))
--   end
--   self.notes = {}
-- end

return Chorder