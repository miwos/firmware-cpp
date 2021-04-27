local utils = require('utils')
Timer.events = {}

function Timer.schedule(time, callback)
  local id = Timer.cpp_schedule(time)
  Timer.events[id] = callback
  return id
end

function Timer.handleEvent(id)
  Log.info('Event!')
  utils.callIfExists(Timer.events[i])
end

-- local Timer = {
--   events = {}
-- }

-- local eventIndex = 0

-- ---Schedule an event.
-- ---@param self table
-- ---@param time number The time of the event in milliseconds.
-- ---@param callback function The callback function.
-- ---@return integer eventId A unqiue id to cancel the event.
-- function Timer:schedule(time, callback)
--   local index = eventIndex
--   eventIndex = eventIndex + 1
--   self.events[index] = { time, callback }
--   return index
-- end

-- ---Cancel an event.
-- ---@param self table
-- ---@param eventId number The unique id returned by `Timer:scheulde()`
-- function Timer:cancel(eventId)
--   if eventId ~= nil then
--     self.events[eventId] = nil
--   end
--   -- Reset `eventIndex` to maintain smaller numbers.
--   if (#self.events == 0) then
--     eventIndex = 0
--   end
-- end

-- function Timer:update(currentTime)
--   for key, event in pairs(self.events) do
--     local time, callback = unpack(event)
--     if currentTime >= time then
--       callback()
--       self.events[key] = nil
--     end
--   end
-- end

-- return Timer