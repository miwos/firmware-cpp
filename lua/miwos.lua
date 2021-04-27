local class = require('class')
local Module = require('module')

local MiwosInput = class(Module)
local MiwosOutput = class(Module)

function MiwosOutput:input(index, message)
  local actions = {
    [Midi.TypeNoteOn] = Midi.sendNoteOn,
    [Midi.TypeNoteOff] = Midi.sendNoteOff,
    [Midi.TypeControlChange] = Midi.sendControlChange
  }

  local action = actions[message.type]
  if action then
    -- Decrease index, because we use zero-based index in c++.
    action(index - 1, unpack(message.payload))
  end
end

Miwos = {
  input = MiwosInput(),
  output = MiwosOutput()
}

function Miwos.Module()
  return class(Module)
end

return Miwos