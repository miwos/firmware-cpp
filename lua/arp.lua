local class = require('class')
local Timer = require('timer')

local Arp = class()

function sendNote(length, note)
  sendNoteOn(unpack(note))
  Timer:schedule(getTime() + length, function ()
    sendNoteOff(unpack(note))
  end)
end

function Arp:init()
  self.notes = {}
  self.noteIndex = 1
  self.lastNoteTime = 0
  self.timerId = nil
end

function Arp:clear()
  info('clear')
  for _, note in pairs(self.notes) do
    sendNoteOff(unpack(note))
  end
  self.notes = {}
  Timer:cancel(self.timerId)
  self.playing = false
end

function Arp:handleNoteOn(...)
  local time = getTime()
  if (time - self.lastNoteTime > 100) then
    self:clear()
  end
  self:addNote({...})
  self.lastNoteTime = time
  if not self.playing then
    self:update()
    self.playing = true
  end
end

function Arp:addNote(note)
  table.insert(self.notes, note)
end

function Arp:update()
  local noteIndex = self.noteIndex

  local note = self.notes[noteIndex]
  -- Check for nil because the notes might have been cleared in the meantime.
  if note ~= nil then sendNote(50, note) end

  local _self = self
  self.timerId = Timer:schedule(getTime() + 100, function ()
    Arp.update(_self)
  end)

  info(effect.timerId)

  self.noteIndex = noteIndex < #self.notes and noteIndex + 1 or 1
end

return Arp