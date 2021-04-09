local function class(base)
  local c = {}

  -- Inherit base by making a shallow copy.
  if type(base) == 'table' then
    for key,value in pairs(base) do c[key] = value end
    c._base = base
  end

  c.__index = c
  local mt = {}

  -- Expose a constructor which can be called by <classname>(<args>).
  mt.__call = function(table, ...)
    local instance = {}
    setmetatable(instance, c)
    if table.init then table.init(instance, ...) end
    return instance
  end

  setmetatable(c, mt)
  return c
end

return class