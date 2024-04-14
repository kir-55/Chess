#ifndef METHODS_H
#define METHODS_H

#include "Moves.h"
#include "ChessConstants.h"

position getKingPos(bool color, std::array<std::array<int, 8>, 8> board);
// sf::Vector2i --> position

bool positionExist(position pos);
// ints --> position

bool checkForCheck(position kingPos, std::array<std::array<int, 8>, 8> attackMap);
// sf::Vector2i --> position

int getPiece(position pos, std::array<std::array<int, 8>, 8> board);
// sf::Vector2i --> position

Piece& getPiece(int piece);

void movePiece(globalMove globalMove, std::array<std::array<int, 8>, 8>& board);
//it no longer returns string, instead new funciton move to string added which takes global move

std::string moveToString(globalMove globalMove);

std::array<std::array<int, 8>, 8> getAttackMap(std::array<std::array<int, 8>, 8> board, bool color, std::string moves);

std::vector<possibleMove> getSafeMoves(std::array<std::array<int, 8>, 8> board, std::vector<possibleMove> moves, position from, bool color, std::string movesNotation);
// sf::Vector2i --> position, board, color, movesNotation

std::vector<globalMove> getAllMoves(std::array<std::array<int, 8>, 8> board, std::array<std::array<int, 8>, 8> attackMap, bool color, std::string movesNotation);
// global chessboard --> local std::array<std::array<int, 8>, 8> board, color

#endif
