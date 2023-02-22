local em = require 'edge.math'

local M = {}
M.BGColor = {77, 170, 182, 200 }
M.DBPos = em.Vec2:new(64, 42)
M.State = {
  MainMenu = 0,
  GameOver = 1,
  Win = 2,
}

local textNormalColor = { 34, 32, 32 }
local textRedColor = { 172, 50, 50 }
local textGreenColor = { 51, 125, 60 }

M.Texts = {
  [M.State.MainMenu] = {
    {"SDL Breakout", 18, textRedColor},
    {"Press Z to play", 34, textNormalColor},
    {"↑ - launch ball", 50, textNormalColor},
    {"← → - move", 66, textNormalColor},
    {"2023, Elias Daler", 196, textNormalColor},
  },
  [M.State.GameOver] = {
    {"panic:game over", 18, textRedColor},
    {"Press Z to", 50, textNormalColor},
    {"recover", 66, textGreenColor},
  },
  [M.State.Win] = {
    {"YOU WON!", 18, textGreenColor},
    {"Press Z to", 50, textNormalColor},
    {"play again", 66, textNormalColor},
  },
}

function M.initTexts(font)
  for _, ts in pairs(M.Texts) do
    for _, t in ipairs(ts) do
      t[1] = edge.DrawableString.new(font, t[1])
    end
  end
end

return M
