-- menu
local Menu = {}

local neutral = Sprite.new()
local rec = Rect.new()
local jump = Sound.new()
local cam = Cam2D.new(Vec2.zero(), 0, 1)

local txt = Text.new()

function Menu.Init()
    neutral:load("assets/sprites/w_icon.png")
    neutral:pos(0, 0)
    neutral:size(128, 128)

    rec:pos(0, 64)
    rec:size(64, 64)
    
    -- txt:load("assets/fonts/Jersey10.ttf")
    txt:spacing(0.7)
    txt:pos(100, 100)
    
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
    neutral:draw()
    txt:draw("Hello\nWorld", 50, white)
    render.cam_end()
end

return Menu
