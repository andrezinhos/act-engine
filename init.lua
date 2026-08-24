eng.fps(60)

eng.start(800, 600, "Window")

local tex = ios.load_tex("assets/sprites/smile.png")
local pos = vec2.zero();
local cam = cam_2d.new(vec2.zero(), 0, 1)

while eng.loop() do
	eng.clear(black)

	if ios.key_down(key.w) then
		pos.y = pos.y - 10
	elseif ios.key_down(key.s) then
		pos.y = pos.y + 10
	elseif ios.key_down(key.a) then
		pos.x = pos.x - 10
	elseif ios.key_down(key.d) then
		pos.x = pos.x + 10
	end
		
    eng.begin_draw()
    eng.cam_begin(cam)

    render.draw_tex(tex, pos, vec2.new(100, 100))

    eng.cam_end()
    eng.end_draw();
end

eng.stop()
