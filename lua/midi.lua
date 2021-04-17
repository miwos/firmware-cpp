local Midi = {
  TypeNoteOn = 1,
  TypeNoteOff = 2,
}

function Midi.NoteOn(note, velocity, channel)
  return Midi.Message(Midi.TypeNoteOn, { note, velocity, channel })
end

function Midi.NoteOff(note, velocity, channel)
  return Midi.Message(Midi.TypeNoteOff, { note, velocity, channel })
end

function Midi.Message(type, payload)
  return { type = type, payload = payload }
end

return Midi