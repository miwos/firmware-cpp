local function callIfExists(fn, args)
  if fn then
    fn(unpack(args))
  end
end

return {
  callIfExists = callIfExists
}