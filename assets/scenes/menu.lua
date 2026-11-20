local Menu = {}

local player = sys.new_ent()
local player_pos = vec2.zero()

function Menu.init()
    sys.ent_pos(player, player_pos)
    sys.ent_scale(player, vec2.set(100, 100))

    sys.load_tex(player, "eng/w_icon.png")
end

function Menu.update(dt)
end

function Menu.draw()
    sys.render(player)
    eng.fps()
end

return Menu
