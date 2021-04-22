local class = require('class')
local Module = require('module')
local Midi = require('midi')
local Timer = require('timer')

local MiwosInput = class(Module)
local MiwosOutput = class(Module)

function MiwosOutput:input(index, message)
  local actions = {
    [Midi.TypeNoteOn] = Teensy.sendNoteOn,
    [Midi.TypeNoteOff] = Teensy.sendNoteOff,
    [Midi.TypeControlChange] = Teensy.sendControlChange
  }

  local action = actions[message.type]
  if action then
    -- Increase index by one, because we use zero-based index in c++.
    action(index - 1, unpack(message.payload))
  end
end

local Miwos = {
  Timer = Timer,
  input = MiwosInput(),
  output = MiwosOutput()
}

function Miwos.Module()
  return class(Module)
end

function Miwos.update(time)
  Timer:update(time)
end

function Miwos.handleNoteOn(...)
  Miwos.input:output(1, Midi.NoteOn(...))
end

function Miwos.handleNoteOff(...)
  Miwos.input:output(1, Midi.NoteOff(...))
end

return Miwos