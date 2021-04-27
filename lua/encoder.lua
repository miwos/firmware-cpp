Encoder = Encoder or {}

function Encoder.handleChange(index, value)
  if effect and index == 0 then
    effect:setParam('interval', value + 100)
    Log.info(string.format('Interval = %d', value + 10))
  end

  if effect and index == 1 then
    effect:setParam('gate', value / 127)
    Log.info(string.format('Gate = %d', value))
  end
end