eng.win_flag(flags.vsync)
eng.fps(60)

eng.start(800, 600, "Window")

-- local tex = ios.load_tex("assets/sprites/smile.png")
local pos = vec3.zero();
local cam = cam_3d.new(vec3.new(0, 0, 5), vec3.zero(), vec3.new(0, 1, 0), 45)

while eng.loop() do
    eng.clear(black)
    eng.begin_draw();
    eng.cam_begin_3d(cam)

    render.draw_cube(pos, vec3.new(10, 10, 10), blue)

    eng.cam_end()
    -- eng.end_draw();
end

eng.stop()
