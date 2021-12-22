#ifndef GAME_INPUT_MODEL_H
#define GAME_INPUT_MODEL_H

struct Input {
public:
    bool isWPressed;
    bool isAPressed;
    bool isSPressed;
    bool isDPressed;

    Input(bool isWPressed, bool isAPressed, bool isSPressed, bool isDPressed) : isWPressed(isWPressed),
                                                                                isAPressed(isAPressed),
                                                                                isSPressed(isSPressed),
                                                                                isDPressed(isDPressed) {}
};
#endif //GAME_INPUT_MODEL_H

