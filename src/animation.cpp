#include "animation.hpp"
#include "mkr.h"
#include "core.hpp"

void anim::PlayAnimation(Sheet& sheet, bool loop){
    if (sheet.end && !loop) return;

    sheet.counter += engine::time::get_delta();

    if (sheet.counter >= sheet.duration){
        sheet.counter -= sheet.duration;
        sheet.currFrame++;

        if (sheet.currFrame >= (int)sheet.frames.size()){
            if (loop){
                sheet.currFrame = 0;
            } else {
                sheet.end = true;
                sheet.currFrame = 0;
            }
        }
    }
}

void anim::RenderAnimation(Sheet& sheet, texture* tex, vec2 position, float size, color color){
    mk_render_tex_region(
        tex,
        sheet.frames[sheet.currFrame],
        position,
        {size, size},
        color
    );
}
