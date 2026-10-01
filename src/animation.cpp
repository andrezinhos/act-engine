#include "animation.hpp"
#include "mkr.hpp"
#include "time.hpp"

void anim::PlayAnimation(Sheet& sheet, bool loop){
    if (sheet.end && !loop) return;

    sheet.counter += Time::GetDelta();

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

void anim::RenderAnimation(Sheet& sheet, Texture* tex, Vec2 position, float size, Color color){
    mkr::RenderTextureRec(
        tex,
        sheet.frames[sheet.currFrame],
        position,
        (Vec2){size, size},
        color
    );
}
