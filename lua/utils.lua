local function callIfExists(fn, args)
  if fn then
    fn(unpack(args or {}))
  end
end

return {
  callIfExists = callIfExists
}