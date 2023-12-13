#pragma once


int boardSizeX = 8, boardSizeY = 8;

const std::string PAWN_WHITE = "pawn_white.png";
const std::string KNIGHT_WHITE = "knight_white.png";
const std::string BISHOP_WHITE = "bishop_white.png";
const std::string ROOK_WHITE = "rook_white.png";
const std::string QUEEN_WHITE = "queen_white.png";
const std::string KING_WHITE = "king_white.png";

const std::string PAWN_BLACK = "pawn_black.png";
const std::string KNIGHT_BLACK = "knight_black.png";
const std::string BISHOP_BLACK = "bishop_black.png";
const std::string ROOK_BLACK = "rock.png";
const std::string QUEEN_BLACK = "queen_black.png";
const std::string KING_BLACK = "king_black.png";

Pawn PAWN{ "pawn", PAWN_WHITE, PAWN_BLACK };
King KING{ "king", KING_WHITE, KING_BLACK};
Piece KNIGHT{ "knight", KNIGHT_WHITE, KNIGHT_BLACK, ' ', { {2,-1}, {2, 1}, {1,2 }, {-1, 2}, {-2, 1}, {-2, -1}, {-1,-2}, {1, -2} } };
Piece BISHOP{ "bishop", BISHOP_WHITE, BISHOP_BLACK, 'X' ,{} };
Piece QUEEN{ "queen", QUEEN_WHITE, QUEEN_BLACK, '*' ,{} };
Piece ROOK{ "rook", ROOK_WHITE, ROOK_BLACK, '+' ,{} };

std::string moveSeparator = "|";
const std::array<char, 8> abc = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
