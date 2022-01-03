#ifndef GAME_INPUT_MODEL_H
#define GAME_INPUT_MODEL_H

struct Input {
public:

    double xPos, yPos;
    bool isWPressed, isAPressed, isSPressed, isDPressed;

    Input(double xPos, double yPos, bool isWPressed, bool isAPressed, bool isSPressed, bool isDPressed)
            : xPos(xPos),
              yPos(yPos),
              isWPressed(isWPressed),
              isAPressed(isAPressed),
              isSPressed(isSPressed),
              isDPressed(isDPressed) {}
};

#endif //GAME_INPUT_MODEL_H

