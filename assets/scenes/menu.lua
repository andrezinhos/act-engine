-- menu
local Menu = {}

local neutral = Sprite.new()
local rec = Rect.new()
local jump = Sound.new()
local cam = Cam2D.new(Vec2.zero(), 0, 1)

local txt = Text.new()

local anim = Anim2D.new()

function Menu.Init()
    neutral:load("eng/w_icon.png")
    neutral:pos(0, 0)
    neutral:size(128, 128)

    rec:pos(0, 64)
    rec:size(64, 64)

    txt:spacing(1.0)
    txt:pos(100, 100)

    jump:load("assets/audio/Jump.wav")

    anim:load("assets/sprites/rgb.png")

    anim:add_frame(0, 0, 64, 64)
    anim:add_frame(64, 0, 64, 64)
    anim:add_frame(0, 64, 64, 64)
    anim:add_frame(64, 64, 64, 64)

    -- render.animation_frames(anim, {
    --     { 0,  0, 64, 64 },
    --     { 64, 0, 64, 64 },
    --     { 0, 64, 64, 64 },
    --     { 64, 64, 64, 64 },
    -- })

    anim:pos(100, 100)

    anim:size(128)

    anim:duration(2)
end

anim.playing = false

function Menu.Update(dt)
    if (ios.key_pressed(key.e)) and not anim.playing then
        -- jump:play()
        eng.change_scene("game.lua")
        -- anim.playing = true
    end

    if (anim.playing) then
        anim:play(true)
    end
end

function Menu.Draw()
    render.cam_begin(cam)
    -- neutral:draw()
    anim:draw()
    eng.get_fps()
    render.cam_end()
end

return Menu
