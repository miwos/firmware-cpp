local Chorder = require('chorder')
local Arp = require('arp')

local types = {
  [1] = Chorder,
  [2] = Arp,
  [3] = Chorder
}

local connections = {
  { 0, 1, 1, 1},
  { 1, 1, 2, 1},
  { 2, 1, 0, 1}
}

local interface = {
  page1 = {
    encoder1 = { 2, 'interval' },
    encoder2 = { 2, 'gate' }
  }
}

return {
  types = types,
  connections = connections,
  interface = interface
}