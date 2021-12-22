#ifndef GAME_FRAME_DATA_MODEL_H
#define GAME_FRAME_DATA_MODEL_H

struct FrameData {
public:
    uint16_t frame = 0;
    double t0 = 0;
    double t1 = 0;

    float getTimeDelta() {
        return float(t1 - t0);
    }
};
#endif //GAME_FRAME_DATA_MODEL_H
