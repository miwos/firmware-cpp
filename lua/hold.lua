local class = require('class')

local Hold = class()

function Hold:init()
  self.notes = {}
  self.lastNoteTime = 0
end

function Hold:clear()
  for _, note in pairs(self.notes) do
    sendNoteOff(unpack(note))
  end
  self.notes = {}
end

function Hold:handleNoteOn(...)
  local time = getTime()
  if (time - self.lastNoteTime > 100) then
    self:clear()
  end
  self:addNote({...})
  self.lastNoteTime = time
end

function Hold:addNote(note)
  table.insert(self.notes, note)
  sendNoteOn(unpack(note))
end

return Hold