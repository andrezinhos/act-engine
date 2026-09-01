-- eng.win_flag(max)
eng.win_flag(resizable)
eng.fps(60)
eng.start(800, 600, "Window")

-- render.cursor(hidden);

local smile = Sprite.new()
local neutral = Sprite.new()

local jump = Sound.new()
local theme = Music.new()

smile:load("assets/sprites/smile.png")
neutral:load("assets/sprites/neutral.png")

smile:size(100, 100)
neutral:size(500, 500)

local posX = (render.win_width() / 2) - (smile.texture.width / 2)
local posY = (render.win_height() / 2) - (smile.texture.height / 2)

smile:pos(posX, posY);
neutral:pos(0, 0)

jump:load("assets/audio/Jump.wav")
theme:load("assets/audio/t1.ogg")

theme:play()

local cam = Cam2D.new(smile.position, 0, 1)
while eng.loop() do
	eng.clear(black)

	if ios.key_down(key.w) then
		smile.position.y = smile.position.y - 500 * eng.delta()
	end
	if ios.key_down(key.s) then
		smile.position.y = smile.position.y + 500 * eng.delta()
	end
	if ios.key_down(key.a) then
		smile.position.x = smile.position.x - 500 * eng.delta()
	end
    if ios.key_down(key.d) then
        smile.position.x = smile.position.x + 500 * eng.delta()
    end

    if ios.key_pressed(key.e) then
        jump:play()
    end

	cam.position.x = smile.position.x - (render.win_width()/2) + (smile.texture.width /2)
	cam.position.y = smile.position.y - (render.win_height()/2) + (smile.texture.height /2)

    render.begin_draw()
    render.cam_begin(cam)

    render.sprite(neutral.id, neutral)
    render.sprite(smile.id, smile)

    render.cam_end()
    render.end_draw();
end

eng.stop()
