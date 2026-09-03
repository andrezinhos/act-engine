-- menu
local Menu = {}

local neutral = Sprite.new()
local rec = Rect.new()
local jump = Sound.new()
local cam = Cam2D.new(Vec2.zero(), 0, 1)

function Menu.Init()
    neutral:load("assets/sprites/rgb.png")
    neutral:pos(0, 0)
    neutral:size(128, 128)

    rec:pos(0, 64)
    rec:size(64, 64)
    
    jump:load("assets/audio/Jump.wav")
end

function Menu.Update(dt)
    if (ios.key_pressed(key.e)) then
        jump:play()
        eng.change_scene("game.lua")
    end
end

function Menu.Draw()
    render.cam_begin(cam)
    neutral:draw_area(rec)
    render.cam_end()
end

return Menu
