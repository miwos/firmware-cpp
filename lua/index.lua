local class = require('classs')

local MyClass = class()
function MyClass:init(x)
  self.x = x
end

function MyClass:test()
  print(self.x)
end

local test = MyClass(10)
test:test()