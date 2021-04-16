local Module = {
  -- The number of inputs.
  inputs = 0,
  -- The number of outputs.
  outputs = 0,
  -- The actual output information, stored as an array of { module, moduleInputIndex }
  _outputs = {}
}

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

  local inputMethod = output.module['input' .. output.input]
  if not inputMethod then return end

  inputMethod(module, message)
end

return Module