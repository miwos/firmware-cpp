local Chorder = require('chorder')
local Arp = require('arp')

return {
  modules = {
    chorder1 = Chorder(),
    arp1 = Arp()
  },
  connections = {
    { Miwos.input, 1, 'chorder1', 1 },
    { 'chorder1', 1, 'arp1', 1 },
    { 'chorder1', 1, Miwos.output, 1 }
  },
  interface = {
    page1 = {
      encoder1 = { 'arp1', 'interval' },
      encoder2 = { 'arp1', 'gate' }
    }
  }
}