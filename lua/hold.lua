local Hold = Miwos.Module()

function Hold:init()
  self.notes = {}
  self.lastNoteTime = 0
end

function Hold:input1_noteOn(message)
  local time = getTime()
  if (time - self.lastNoteTime > 100) then
    self:clear()
  end
  self:addNote(message.payload)
  self.lastNoteTime = time
end

function Hold:addNote(note)
  table.insert(self.notes, note)
  self:output(1, Midi.NoteOn(unpack(note)))
end

function Hold:clear()
  for _, note in pairs(self.notes) do
    self:output(1, Midi.NoteOff(unpack(note)))
  end
  self.notes = {}
end

return Hold