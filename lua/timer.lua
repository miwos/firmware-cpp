local utils = require('utils')
Timer.events = {}

---Schedule an event.
---@param time number The time of the event in milliseconds.
---@param callback function The callback function.
---@return integer eventId A unqiue id to cancel the event.
function Timer.schedule(time, callback)
  local eventId = Timer._schedule(time)
  Timer.events[eventId] = callback
  return eventId
end

---Cancel an event.
---@param eventId number The unique id returned by `Timer:scheulde()`
function Timer.cancel(eventId)
  if eventId == nil then return end
  Timer._cancel(eventId)
  Timer.events[eventId] = nil
end

function Timer.handleEvent(eventId)
  utils.callIfExists(Timer.events[eventId])
  Timer.events[eventId] = nil
end