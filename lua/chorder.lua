local Chorder = Miwos.Module()

Chorder._class = "Chorder"

-- Chorder:input1_noteOn()
-- Chorder:input1_noteOff()
-- Chorder:input1_CC()

function Chorder:init()
  self.notes = {}
  self.inputs = 1
  self.outputs = 1
  self.pitches = { 3, 7 }
end

function Chorder:input1_noteOn(message)
  self:output(1, message)
  local note, velocity, channel = unpack(message.payload)
  self:sendChordNote(note + 3, velocity, channel)
  self:sendChordNote(note + 7, velocity, channel)
end

function Chorder:input1_noteOff(message)
  self:output(1, message)
  self:clear()
end

function Chorder:sendChordNote(...)
  table.insert(self.notes, {...})
  self:output(1, Midi.NoteOn(...))
end

function Chorder:clear()
  for _, note in pairs(self.notes) do
    self:output(1, Midi.NoteOff(unpack(note)))
  end
  self.notes = {}
end

return Chorder