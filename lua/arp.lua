local Arp = Miwos.Module()

function Arp:init()
  self.notes = {}
  self.noteIndex = 1
  self.lastNoteTime = 0
  self.timerId = nil
end

function Arp:input1_noteOn(message)
  local time = getTime()
  if (time - self.lastNoteTime > 100) then
    self:clear()
  end
  self:addNote(message.payload)
  self.lastNoteTime = time
  if not self.playing then
    self:update()
    self.playing = true
  end
end

function Arp:update()
  local noteIndex = self.noteIndex

  local note = self.notes[noteIndex]
  -- Check for nil because the notes might have been cleared in the meantime.
  if note ~= nil then
    self:output(1, Midi.NoteOn(unpack(note)))
    Miwos.Timer:schedule(getTime() + 50, function ()
      self:output(1, Midi.NoteOff(unpack(note)))
    end)
  end

  local _self = self
  self.timerId = Miwos.Timer:schedule(getTime() + 500, function ()
    Arp.update(_self)
  end)

  self.noteIndex = noteIndex < #self.notes and noteIndex + 1 or 1
end

function Arp:addNote(note)
  table.insert(self.notes, note)
end

function Arp:clear()
  for _, note in pairs(self.notes) do
    self:output(1, Midi.NoteOn(unpack(note)))
  end
  self.notes = {}
  Miwos.Timer:cancel(self.timerId)
  self.playing = false
end

return Arp