#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include <iostream>


class Pawn : public Piece
{
public:
    Pawn(std::string name, std::string whiteSpriteName, std::string blackSpriteName);
    std::vector<possibleMove> GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::array<std::array<int, 8>, 8> attackMap, std::string moves, position pos, bool onlyAttack = false) override;
    void AfterMove(std::array<std::array<int, 8>, 8>& board, possibleMove move) override;
};

#endif