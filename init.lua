eng.win_flag(flags.vsync)

eng.start(800, 600, "Window")

local tex = ios.load_tex("assets/sprites/smile.png")
local pos = vec2.new(100, 100)
local cam = cam_2d.new(vec2.zero(), 0, 1)

while eng.loop() do

    if (ios.key_down(key.w)) then
        pos.y = pos.y - 10
    elseif (ios.key_down(key.s)) then
        pos.y = pos.y + 10
    elseif (ios.key_pressed(key.a)) then
        pos.x = pos.x - 20
    elseif (ios.key_pressed(key.d)) then
        pos.x = pos.x + 20
    end
    eng.clear(color.black)
    eng.begin_draw();
    eng.cam_begin(cam)

    render.draw_tex(tex, pos, vec2.new(100, 100))

    eng.cam_end()
    eng.end_draw();
end

eng.stop()
