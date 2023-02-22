local class = require 'lib.middleclass'

local Vec2 = class("Vec2")
function Vec2:initialize(x, y)
  self.x = x or 0
  self.y = y or 0
end

local Rect = class("Rect")
function Rect:initialize(x, y, w, h)
  self.x = x or 0
  self.y = y or 0
  self.w = w or 0
  self.h = h or 0
end

-- Checks if two rects collide and returns a depth of intersection
local function getIntersectionDepth(rectA, rectB)
  if rectA.w == 0 or rectB.w == 0 or
     rectB.w == 0 or rectB.w == 0 then
     return Vec2:new()
  end

  local rectARight = rectA.x + rectA.w
  local rectADown = rectA.y + rectA.h

  local rectBRight = rectB.x + rectB.w
  local rectBDown = rectB.y + rectB.h

  if rectA.x >= rectBRight or rectARight <= rectB.x or
     rectA.y >= rectBDown or rectADown <= rectB.y then
     return Vec2:new()
  end

  local depth = Vec2:new()

  -- x
  if rectA.x < rectB.x then
    depth.x = rectARight - rectB.x
  else
    depth.x = rectA.x - rectBRight
  end

  -- y
  if rectA.y < rectB.y then
    depth.y = rectADown - rectB.y
  else
    depth.y = rectA.y - rectBDown
  end

  return depth
end

return {
  Vec2 = Vec2,
  Rect = Rect,
  getIntersectionDepth = getIntersectionDepth,
}
