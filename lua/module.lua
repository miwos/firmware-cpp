local utils = require('utils')
local Module = {}

local midiTypeNames = {
  'noteOn',
  'noteOff'
}

function Module:init()
  -- The number of inputs.
  self.inputs = 0
  -- The number of outputs.
  self.outputs = 0
  -- The actual output information, stored as an array of { module, moduleInputIndex }
  self._outputs = {}
end

---Connect an output to the input of another module.
---@param output number
---@param module table
---@param input number
function Module:connect(output, module, input)
  self._outputs[output] = { module, input }
end

---Send data to output.
---@param index number
---@param message table
function Module:output(index, message)
  local output = self._outputs[index]
  if not output then return end

  local module, input = unpack(output)

  -- Call a midi-type agnostic function like `input1()`.
  local numberedInput = 'input' .. input
  utils.callIfExists(module[numberedInput], { module, message })
  info(numberedInput)

  -- Call a midi-type aware function like `input1_noteOn()`.
  local midiType =  midiTypeNames[message.type]
  utils.callIfExists(module[numberedInput .. '_' .. midiType], { module, message })

  -- Call a generic `input()` function that handles any input.
  utils.callIfExists(module.input, { module, input, message })
end

return Module