-- game
local Game = {}

local smile = Sprite.new()
local theme = Music.new()
local cam = Cam2D.new(Vec2.zero(), 0, 1)

function Game.Init()
    smile:load("assets/sprites/smile.png")
    smile:size(100, 100)
    
    local posX = (render.win_width() / 2) - (smile.source.width / 2)
    local posY = (render.win_height() / 2) - (smile.source.height / 2)
    
    theme:load("assets/audio/t1.ogg")
    smile:pos(posX, posY);
    theme:play()
end

function Game.Update(dt)
    if ios.key_down(key.w) then
        smile.position.y = smile.position.y - 500 * dt
    end
    if ios.key_down(key.s) then
        smile.position.y = smile.position.y + 500 * dt
    end
    if ios.key_down(key.a) then
        smile.position.x = smile.position.x - 500 * dt
    end
    if ios.key_down(key.d) then
        smile.position.x = smile.position.x + 500 * dt
    end
end

function Game.Draw()
    render.cam_begin(cam)
    smile:draw()
    render.cam_end()
end

function Game.Exit()
    theme:stop()
end

return Game
