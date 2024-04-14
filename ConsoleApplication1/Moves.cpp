#include "Moves.h"

enum MoveType {
    MOVE,
    CAPTURE,
    EN_PASSANT,
    CASTLE,
    PROMOTION
};

struct possibleMove {
    int x;
    int y;
    MoveType moveType;
};

struct globalMove : public possibleMove {
    int fromX;
    int fromY;
};

struct moveValue : public globalMove {
    float value;
};