eng.win_flag(flags.resizable)
eng.fps(60)
eng.start(800, 600, "Window")

local tex = ios.load_tex("assets/sprites/smile.png")
local tex2 = ios.load_tex("assets/sprites/neutral.png")

tex.width = 100
tex.height = 100

local posX = (eng.win_width() / 2) - (tex.width / 2) 
local posY = (eng.win_height() / 2) - (tex.height / 2) 

local pos = vec2.new(posX, posY);
local cam = cam_2d.new(pos, 0, 1)

while eng.loop() do
	eng.clear(black)

	if ios.key_down(key.w) then
		pos.y = pos.y - 500 * eng.delta()
	end
	if ios.key_down(key.s) then
		pos.y = pos.y + 500 * eng.delta()
	end
	if ios.key_down(key.a) then
		pos.x = pos.x - 500 * eng.delta()
	end
	if ios.key_down(key.d) then
		pos.x = pos.x + 500 * eng.delta()
	end

	cam.position.x = pos.x - (eng.win_width()/2) + (tex.width /2)
	cam.position.y = pos.y - (eng.win_height()/2) + (tex.height /2)

    eng.begin_draw()
    eng.cam_begin(cam)

    render.draw_tex(tex2, vec2.zero(), vec2.new(600, 600))
    render.draw_tex(tex, pos, vec2.new(100, 100))

    eng.cam_end()
    eng.end_draw();
end

eng.stop()
