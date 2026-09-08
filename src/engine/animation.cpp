#include "animation.hpp"
#include "core.hpp"
#include "mkr.hpp"

void anim::PlayAnimation(Sheet& anim, bool loop){
    if (anim.end) return;

    anim.counter += core::GetDelta();

    if (anim.counter >= anim.duration){
        anim.counter = 0.0;
        anim.currFrame++;

        if (anim.currFrame >= static_cast<int>(anim.frames.size())){
            if (loop){
                anim.currFrame = 0;
            } else {
                anim.end = true;
                anim.currFrame = 0;
            }
        }
    }
}

std::vector<Rectangle> anim::SetAnimationFrames(const std::vector<Rectangle>& frames){
    return frames;
}

void anim::RenderAnimation(Sheet &sheet, Texture* tex, Vec2 position, float size, Color color){
    if (sheet.end) return;

    mkr::RenderTextureRec(
        tex,
        sheet.frames[sheet.currFrame],
        position,
        { size, size },
        color
    );
}
