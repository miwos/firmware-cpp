Modules = {}

require('timer')
require('encoder')
require('midi')
require('miwos')

local patch = require('patch')

local context = {
  modules = {}
}

for id, Type in pairs(patch.types) do
  context.modules[id] = Type(context)
end

collectgarbage('collect')
print(collectgarbage('count'))

for id, Type in pairs(patch.types) do
  context.modules[id] = Type(context)
end

for _, connection in pairs(patch.connections) do
  local fromId, output, toId, input = unpack(connection)

  local fromModule = fromId == 0 and Miwos.input or context.modules[fromId]
  local toModule = toId == 0 and Miwos.output or context.modules[toId]

  fromModule:connect(output, toModule, input)
end