-- menu
local Menu = {}

local neutral = Sprite.new()
local jump = Sound.new()
local cam = Cam2D.new(Vec2.zero(), 0, 1)

function Menu.Init()
    neutral:load("assets/sprites/neutral.png")
    neutral:pos(0, 0)
    neutral:size(500, 500)
    
    jump:load("assets/audio/Jump.wav")
end

function Menu.Update()
    if (ios.key_pressed(key.e)) then
        jump:play()
        eng.change_scene("game.lua")
    end
end

function Menu.Draw()
    render.cam_begin(cam)
    neutral:draw()
    render.cam_end()
end

return Menu
