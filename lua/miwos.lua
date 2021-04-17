local class = require('class')
local Module = require('module')
local Midi = require('midi')
local Timer = require('timer')

local MiwosInput = class(Module)
local MiwosOutput = class(Module)

function MiwosOutput:input(_, message)
  local actions = {
    [Midi.TypeNoteOn] = Teensy.sendNoteOn,
    [Midi.TypeNoteOff] = Teensy.sendNoteOff
  }

  local action = actions[message.type]
  if action then
    action(unpack(message.payload))
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