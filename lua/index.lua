Miwos = require('miwos')
Midi = require('midi')

local Chorder = require('chorder')

local chorder = Chorder()
Miwos:connect(1, chorder, 1)
chorder:connect(1, Miwos, 1)