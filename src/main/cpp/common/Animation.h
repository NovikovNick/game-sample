#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

namespace Animation {

    struct Properties {
        const uint8_t FPS;
        const float velocity;

        Properties(float velocity = 1, uint8_t FPS = 60) : velocity(velocity), FPS(FPS) {}
    };

    float bounce(float timeFraction) { return timeFraction * timeFraction * (4 * timeFraction - 3); }

    float getTimeFraction(uint16_t frameSeqNumber, const Properties &props) {
        float frameThreshold = props.FPS / props.velocity;
        float timeFraction = (frameSeqNumber % (uint16_t) frameThreshold) / frameThreshold;
        return timeFraction;
    }

    float animateLinear(const float from,
                        const float to,
                        const uint16_t frameSeqNumber,
                        const Properties &props = Properties()) {

        return from + (to - from) * getTimeFraction(frameSeqNumber, props);
    }

    float animateBack(const float from,
                      const float to,
                      const uint16_t frameSeqNumber,
                      const Properties &props = Properties()) {

        return from + (to - from) * bounce(getTimeFraction(frameSeqNumber, props));
    }
}


#endif //GAME_ANIMATION_H
