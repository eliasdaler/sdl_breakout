local class = require 'lib.middleclass'

local SoundCache = class("SoundCache")

function SoundCache:initialize()
  self.sounds = {}
end

function SoundCache:loadSound(path, tag)
  if self.sounds[tag] then
    error(string.format("sound with tag %q was already registered", tag))
  end

  self.sounds[tag] = assert(edge.loadSound(path))
end

function SoundCache:playSound(tag)
  local sound = self.sounds[tag]
  if not sound then
    error(string.format("sound with tag %q was not loaded", tag))
  end
  sound:play()
end

return SoundCache
