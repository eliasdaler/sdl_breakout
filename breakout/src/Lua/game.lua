local class = require 'lib.middleclass'

local em = require 'edge.math'
local Entity = require 'edge.entity'
local SoundCache = require 'edge.sound_cache'

local menu = require 'menu'
local logic = require 'logic'

local Game = class("Game")

local State = {
  Menu = 0,
  Playing = 1,
}

function Game:initialize()
  self.state = State.Playing
  self.menuState = menu.State.MainMenu
  self.soundCache = SoundCache:new()

  self.border = em.Rect:new(32, 16, 192, 244)

  self.bg = Entity:new {
    path = "res/images/bg.png"
  }

  -- ball
  local ball = Entity:new{
    path = "res/images/ball.png",
    pos = em.Vec2:new(100, 120)
  }
  ball.sprite:setTextureRect(0, 0, 16, 16)
  ball.speed = em.Vec2:new(90, 90)
  self.ball = ball

  -- reused for all blocks
  local blockTexture = assert(edge.loadTexture("res/images/block.png"))

  -- init blocks
  local blockMarginX = 48
  local blockMarginY = 48
  local numBlocksX = 5
  local numBlocksY = 4
  self.blocks = {}
  for y=1,numBlocksY do
    for x=1,numBlocksX do
      local pos = {
        x = blockMarginX + (x-1)*32,
        y = blockMarginY + (y-1)*16,
      }
      local block = Entity:new{
        texture = blockTexture,
        pos = pos,
      }
      block.isBroken = false
      block.texture = blockTexture
      block.sprite = edge.Sprite.new(block.texture)
      table.insert(self.blocks, block)
    end
  end

  -- paddle
  local paddle = Entity:new{
    path = "res/images/paddle.png",
  }
  paddle.speed = em.Vec2:new(90, 0)
  self.paddle = paddle

  -- menu
  self.font = assert(edge.loadFont("res/JF-Dot-MPlus10.ttf", 10))
  self:initMenu(self.font)

  self:showMenu(menu.State.MainMenu)
  self:reset()

  -- load sounds
  local sounds = {
    "ball_collision",
    "ball_collision_paddle",
    "ball_fall",
  }
  for _, sn in ipairs(sounds) do
    local path = "res/sounds/" .. sn .. ".wav"
    self.soundCache:loadSound(path, sn)
  end

  -- load music
  self.music = assert(edge.loadMusic("res/music/song.ogg"))
  self.music:play()
end

function Game:onStartGame()
  self:reset()
  self.state = State.Playing
end

function Game:reset()
  self.paddle.pos = em.Vec2:new(88, 208)

  self.ball.vel = em.Vec2:new()
  self.ballAttachedToPaddle = true
  logic.setBallOnPaddle(self.ball, self.paddle)

  for _, b in ipairs(self.blocks) do
    b.isBroken = false
  end
end

function Game:update(dt)
  self:processInput()
  if self.state == State.Playing then
    self:updateGame(dt)
  end
  self:testImGuiWindow()
end

function Game:processInput()
  if self.state == State.Menu then
    self:processMenuInput()
  else
    self:processGameInput()
  end
end

function Game:processGameInput()
  local dx = 0
  if edge.isKeyPressed(edge.KEY_LEFT) then
    dx = dx - 1
  end
  if edge.isKeyPressed(edge.KEY_RIGHT) then
    dx = dx + 1
  end
  self.paddle.vel.x = self.paddle.speed.x * dx

  if self.ballAttachedToPaddle then
    if edge.isKeyPressed(edge.KEY_UP) then
      -- launch ball
      self.ballAttachedToPaddle = false
      self.ball.vel = em.Vec2:new(self.ball.speed.x, -self.ball.speed.y)
      self.soundCache:playSound("ball_collision_paddle")
    end
  end
end

function Game:updateGame(dt)
  local ball = self.ball
  local border = self.border
  local paddle = self.paddle

  if logic.checkBallLost(ball, border) then
    self.soundCache:playSound("ball_fall")
    self:showMenu(menu.State.GameOver)
    do return end
  end

  if logic.checkGameWin(self.blocks) then
    ball.vel = em.Vec2:new()
    paddle.vel = em.Vec2:new()
    self:showMenu(menu.State.Win)
    do return end
  end

  if self.ballAttachedToPaddle then
    logic.setBallOnPaddle(ball, paddle)
  end

  -- update paddle
  paddle.pos.x = paddle.pos.x + paddle.vel.x * dt
  paddle.pos = logic.checkBorderCollision(paddle, border)

  -- handle ball collision
  if logic.handleBallPaddleCollision(ball, paddle) then
    self.soundCache:playSound("ball_collision_paddle")
  end
  if logic.handleBallBorderCollision(ball, border) then
    self.soundCache:playSound("ball_collision")
  end
  if logic.handleBallBlocksCollision(ball, self.blocks) then
    self.soundCache:playSound("ball_collision")
  end

  ball.pos.x = ball.pos.x + ball.vel.x * dt
  ball.pos.y = ball.pos.y + ball.vel.y * dt
end

local testString = "hello"

function Game:testImGuiWindow()
  im.Begin("Hello")
    im.Text(string.format("pos: (%.2f, %.2f)", self.ball.pos.x, self.ball.pos.y))
    local ok, str = im.InputText("name", testString)
    if ok then
      testString = str
    end
    im.End()
end

function Game:draw()
  self.bg:draw()
  for _, b in ipairs(self.blocks) do
    if not b.isBroken then
      b:draw()
    end
  end
  self.paddle:draw()
  self.ball:draw()

  if self.state == State.Menu then
    self:drawMenu()
  end
end

--------------------------------
-- Menu stuff
--------------------------------
function Game:initMenu(font)
  self.dialogueBox = Entity:new{
    path = "res/images/dialogue_box.png",
    pos = menu.DBPos,
  }
  menu.initTexts(font)
end

function Game:processMenuInput()
  if edge.isKeyPressed(edge.KEY_Z) then
    self:onStartGame()
  end
end

function Game:showMenu(menuState)
  self.state = State.Menu
  self.menuState = menuState
end

function Game:drawMenu()
  edge.drawRect(0, 0, 256, 240, table.unpack(menu.BGColor))
  self.dialogueBox:draw()

  local texts = menu.Texts[self.menuState]
  for _, t in ipairs(texts) do
    local str = t[1]
    local posY = menu.DBPos.y + t[2]
    local color = t[3]
    edge.drawString(str, menu.DBPos.x + 8, posY - 8, table.unpack(color))
  end
end

return Game:new()
