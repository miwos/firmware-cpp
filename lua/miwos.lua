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
    -- Decrease index, because we use zero-based index in c++.
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

function Miwos.handleTimerEvent(index)
  Log.info('event!')
end

function Miwos.handleNoteOn(input, ...)
  Miwos.input:output(input + 1, Midi.NoteOn(...))
end

function Miwos.handleNoteOff(input, ...)
  Miwos.input:output(input + 1, Midi.NoteOff(...))
end

return Miwos