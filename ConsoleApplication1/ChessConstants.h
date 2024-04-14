#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <array>
#include "Piece.h"
#include "Pawn.h"
#include "King.h"
#include "Moves.h"

const int boardSizeX = 8, boardSizeY = 8;

const std::string CONTENT_PATH = "Content/";

const std::string PAWN_WHITE = CONTENT_PATH + "white-pawn.png";
const std::string KNIGHT_WHITE = CONTENT_PATH + "white-knight.png";
const std::string BISHOP_WHITE = CONTENT_PATH + "white-bishop.png";
const std::string ROOK_WHITE = CONTENT_PATH + "white-rook.png";
const std::string QUEEN_WHITE = CONTENT_PATH + "white-queen.png";
const std::string KING_WHITE = CONTENT_PATH + "white-king.png";

const std::string PAWN_BLACK = CONTENT_PATH + "black-pawn.png";
const std::string KNIGHT_BLACK = CONTENT_PATH + "black-knight.png";
const std::string BISHOP_BLACK = CONTENT_PATH + "black-bishop.png";
const std::string ROOK_BLACK = CONTENT_PATH + "black-rook.png";
const std::string QUEEN_BLACK = CONTENT_PATH + "black-queen.png";
const std::string KING_BLACK = CONTENT_PATH + "black-king.png";

static Pawn PAWN{ "pawn", PAWN_WHITE, PAWN_BLACK };
static King KING{ "king", KING_WHITE, KING_BLACK};
static Piece KNIGHT{ "knight", KNIGHT_WHITE, KNIGHT_BLACK, ' ', { {2,-1}, {2, 1}, {1,2 }, {-1, 2}, {-2, 1}, {-2, -1}, {-1,-2}, {1, -2} } };
static Piece BISHOP{ "bishop", BISHOP_WHITE, BISHOP_BLACK, 'X' ,{} };
static Piece QUEEN{ "queen", QUEEN_WHITE, QUEEN_BLACK, '*' ,{} };
static Piece ROOK{ "rook", ROOK_WHITE, ROOK_BLACK, '+' ,{} };

const std::string moveSeparator = "|";
const std::array<char, 8> abc = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

//to simplify piece's id's are the same as their value
enum Pieces {
    PAWN_ID = 1,
    KNIGHT_ID = 3,
    BISHOP_ID = 4, //bishop value is 4!
    ROOK_ID = 5,
    QUEEN_ID = 9,
    KING_ID = 10
};

#endif