#pragma once


int boardSizeX = 8, boardSizeY = 8;

const std::string SPRITES_PATH = "Sprites/";

const std::string PAWN_WHITE = SPRITES_PATH + "white-pawn.png";
const std::string KNIGHT_WHITE = SPRITES_PATH + "white-knight.png";
const std::string BISHOP_WHITE = SPRITES_PATH + "white-bishop.png";
const std::string ROOK_WHITE = SPRITES_PATH + "white-rook.png";
const std::string QUEEN_WHITE = SPRITES_PATH + "white-queen.png";
const std::string KING_WHITE = SPRITES_PATH + "white-king.png";

const std::string PAWN_BLACK = SPRITES_PATH + "black-pawn.png";
const std::string KNIGHT_BLACK = SPRITES_PATH + "black-knight.png";
const std::string BISHOP_BLACK = SPRITES_PATH + "black-bishop.png";
const std::string ROOK_BLACK = SPRITES_PATH + "black-rook.png";
const std::string QUEEN_BLACK = SPRITES_PATH + "black-queen.png";
const std::string KING_BLACK = SPRITES_PATH + "black-king.png";

Pawn PAWN{ "pawn", PAWN_WHITE, PAWN_BLACK };
King KING{ "king", KING_WHITE, KING_BLACK};
Piece KNIGHT{ "knight", KNIGHT_WHITE, KNIGHT_BLACK, ' ', { {2,-1}, {2, 1}, {1,2 }, {-1, 2}, {-2, 1}, {-2, -1}, {-1,-2}, {1, -2} } };
Piece BISHOP{ "bishop", BISHOP_WHITE, BISHOP_BLACK, 'X' ,{} };
Piece QUEEN{ "queen", QUEEN_WHITE, QUEEN_BLACK, '*' ,{} };
Piece ROOK{ "rook", ROOK_WHITE, ROOK_BLACK, '+' ,{} };

std::string moveSeparator = "|";
const std::array<char, 8> abc = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
