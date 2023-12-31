#pragma once
#include <vector>
#include <array>
#include <string>

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

class Piece
{
private:
	char attackPattern = ' '; // allowed: |, -, ^, <, >, /, \, +, X
	std::vector<std::array<int, 2>> localAttackSpots;
    std::vector<possibleMove> CheckLine(std::array<std::array<int, 8>, 8> board, std::array<int, 2> position, std::array<int, 2> direction, bool onlyAttack = false);
public:
	Piece(std::string name, std::string whiteSpriteName, std::string blackSpriteName, char attackPattern = ' ', std::vector<std::array<int, 2>> localAttackSpots = {});
	virtual std::vector<possibleMove> GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::string moves, std::array<int, 2> position, bool onlyAttack = false);
    virtual void AfterMove(std::array<std::array<int, 8>, 8>& board, possibleMove move);
    std::string name;
	std::string blackSpriteName, whiteSpriteName; // like "pawn.png"
};

