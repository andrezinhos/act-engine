local Menu = {}

local player = sys.new_ent()
local player_pos = vec2.zero()

function Menu.init()
    sys.ent_pos(player, player_pos)
    sys.ent_scale(player, vec2.set(100, 100))

    sys.load_tex(player, "assets/sprites/rgb.png")

    sys.new_animation(player, 4, 0.6);

    sys.add_frame(player, 0, 0, 64, 64)
    sys.add_frame(player, 64, 0, 64, 64)
    sys.add_frame(player, 0, 64, 64, 64)
    sys.add_frame(player, 64, 64, 64, 64)
end

local playing = false

function Menu.update(dt)
    if (eng.key_pressed(key.e)) then
        playing = true
    end

    if playing then
        sys.play_anim(player, false)
    end
end

function Menu.draw()
    sys.render_anim(player)
end

return Menu
