local Menu = {}

local player = sys.new_ent()
local player2 = sys.new_ent()

function Menu.init()
    sys.ent_pos(player, vec2.set(10, 10))
    sys.ent_scale(player, vec2.set(100, 100))

    sys.ent_pos(player2, vec2.set(20, 20));
    sys.ent_scale(player2, vec2.set(200, 200))
end

function Menu.update(dt)
end

function Menu.draw()
end

return Menu
