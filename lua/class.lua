local function class(super)
  local obj = {}
  obj.__index = obj

  -- Inherit base by making a shallow copy.
  if type(super) == 'table' then
    for key,value in pairs(super) do obj[key] = value end
    obj.super = super
  end

  setmetatable(obj, {
    __call = function (_, ...)
      local instance = setmetatable({}, obj)
      if instance.init then instance:init(...) end
      return instance
    end
  })

  return obj
end

return class