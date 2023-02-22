local em = require 'edge.math'

local M = {}

function M.checkBorderCollision(e, border)
  local ew = e:getAABB().w
  local newPos = em.Vec2:new(e.pos.x, e.pos.y)

  -- left wall
  if e.pos.x < border.x then
    newPos.x = border.x
  end

  -- right wall
  if e.pos.x + ew > border.x + border.w then
    newPos.x = border.x + border.w - ew
  end

  -- upper wall
  if e.pos.y < border.y then
    newPos.y = border.y
  end

  return newPos
end

function M.setBallOnPaddle(ball, paddle)
  local paddleW = paddle:getAABB().w
  local ballAABB = ball:getAABB()

  local ballX = (paddle.pos.x + paddleW / 2) - ballAABB.w / 2
  local ballY = paddle.pos.y - ballAABB.h
  ball.pos = em.Vec2:new(ballX, ballY)
  ball.vel = em.Vec2:new()
end

function M.handleBallBorderCollision(ball, border)
  local newPos = M.checkBorderCollision(ball, border)
  if newPos.x ~= ball.pos.x or newPos.y ~= ball.pos.y then
     if ball.pos.x ~= newPos.x then -- hit horizontal wall
       ball.vel.x = -ball.vel.x
     end
     if ball.pos.y ~= newPos.y then -- hit ceiling
       ball.vel.y = -ball.vel.y
     end
     ball.pos = newPos
     return true
  end
  return false
end

-- Returns an angle at which the ball will be launched from the paddle
-- If the ball hits the paddle at leftmost point, the angle will be 3*pi/4
-- If the ball hits the rightmost point, the angle will be pi/4
-- If the ball hits the center, the angle will be pi/2
function M.getBallReflectionAngle(ball, paddle)
  local pW = paddle:getAABB().w
  local pRX = paddle.pos.x + pW
  local bCX = ball.pos.x + ball:getAABB().w/2 -- ball center

  local f = (pRX - bCX) / pW -- right paddle edge = 0, left paddle edge = 1
  f = math.max(0, math.min(f, 1)) -- clamp to [0;1]

  return math.pi/4 + f*math.pi/2 -- project [0;1] to [1/4*math.pi; 3/4*math.pi]
end

function M.getNewVelocity(angle, speed)
  local s = math.sqrt(speed.x*speed.x + speed.y*speed.y)
  return em.Vec2:new(s * math.cos(angle), -s * math.sin(angle))
end

function M.handleBallPaddleCollision(ball, paddle)
  local ballAABB = ball:getAABB()
  local paddleAABB = paddle:getAABB()

  local d = em.getIntersectionDepth(ballAABB, paddleAABB)
  if d.x == 0 and d.y == 0 or
     ballAABB.y + ballAABB.h > paddleAABB.y + paddleAABB.h/2 then
     return false
  end

  ball.pos.y = ball.pos.y -d.y
  local angle = M.getBallReflectionAngle(ball, paddle)
  ball.vel = M.getNewVelocity(angle, ball.speed)
  return true
end

function M.handleBallBlocksCollision(ball, blocks)
  local ballAABB = ball:getAABB()

  for _, b in ipairs(blocks) do
    if b.isBroken then
      goto continue
    end

    local d = em.getIntersectionDepth(ballAABB, b:getAABB())
    if d.x == 0 and d.y == 0 then -- no collision
      goto continue
    end

    b.isBroken = true
    if math.abs(d.y) < math.abs(d.x) then -- collided from top or bottom
      ball.vel.y = -ball.vel.y
    else -- collided from left or right
      ball.vel.x = -ball.vel.x
    end
    do return true end
    ::continue::
  end
  return false
end

function M.checkBallLost(ball, border)
  return ball.pos.y > border.y + border.h
end

function M.checkGameWin(blocks)
  for _, b in ipairs(blocks) do
    if b.isBroken == false then
      return false
    end
  end
  return true
end

return M
