#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H
namespace Animation {

    float animate(float from, float to, uint16_t frameSeqNumber, float velocity = 1) {
        float frameThreshold = 60 / velocity;
        return from + (to - from) * ((frameSeqNumber % (uint16_t) frameThreshold) / frameThreshold);
    }

    // TODO add several animations, animationSettings
}
#endif //GAME_ANIMATION_H
