#ifndef MOVES_H
#define MOVES_H

enum MoveType {
    MOVE,
    CAPTURE,
    EN_PASSANT,
    CASTLE,
    PROMOTION
};

struct position {
    int x;
    int y;
};

struct possibleMove {
    position to;
    MoveType moveType;
};

struct globalMove : public possibleMove {
    position from;
};

struct moveValue : public globalMove {
    float value;
};
#endif