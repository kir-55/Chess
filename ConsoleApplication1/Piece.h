#pragma once
#include <vector>
#include <array>
#include <string>

using namespace std;

class Piece
{
	public:
		Piece(string name, string whiteSpriteName, string blackSpriteName, char attackPattern, vector<array<int, 2>> localAttackSpots);
		string name;
		string blackSpriteName, whiteSpriteName; // like "pawn.png"
		char attackPattern = ' '; // allowed: |, -, ^, <, >, /, \, +, X
		vector<array<int, 2>> localAttackSpots;
};

