#pragma once
#include <vector>
#include <array>
#include <string>

using namespace std;

class Piece
{
private:
	char attackPattern = ' '; // allowed: |, -, ^, <, >, /, \, +, X
	vector<array<int, 2>> localAttackSpots;
public:
	Piece(string name, string whiteSpriteName, string blackSpriteName, char attackPattern, vector<array<int, 2>> localAttackSpots);
	virtual vector<array<int, 2>> getAttackedSquares(array<array<int, 8>, 8> board, array<int, 2> position);
	string name;
	string blackSpriteName, whiteSpriteName; // like "pawn.png"
};

