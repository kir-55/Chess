#pragma once
#include "Piece.h"
#include <string>
class King : public Piece
{
public:
	King(std::string name, std::string whiteSpriteName, std::string blackSpriteName);
	std::vector<possibleMove> GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::string moves, std::array<int, 2> position, bool onlyAttack = false) override;
	void AfterMove(std::array<std::array<int, 8>, 8>& board, possibleMove move) override;
};

