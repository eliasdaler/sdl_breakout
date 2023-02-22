local class = require 'lib.middleclass'
local em = require 'edge.math'

local Entity = class('Entity')

function Entity:initialize(ps)
  local pos = ps.pos or em.Vec2:new(0, 0)
  self.pos = em.Vec2:new(pos.x, pos.y)
  self.vel = em.Vec2:new()

  -- load sprite/texture
  local texture = ps.texture
  if texture == nil then
    texture = assert(edge.loadTexture(ps.path))
  end
  self.texture = texture
  self.sprite = edge.Sprite.new(texture)
end

function Entity:getAABB()
  local _, _, w, h = self.sprite:getTextureRect()
  return em.Rect:new(self.pos.x, self.pos.y, w, h)
end

function Entity:draw()
  edge.drawSprite(self.sprite, self.pos.x, self.pos.y)
end

return Entity
