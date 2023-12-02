#pragma once
#include <vector>;
#include <string>;

using namespace std;


class Piece
{
	public:
		Piece(string name, string blackSpriteName, string whiteSpriteName, char attackPattern, vector<int[2]> localAttackSpots);
		string name;
		string blackSpriteName, whiteSpriteName; // like "pawn.png"
		char attackPattern = ' '; // allowed: |, -, ^, <, >, /, \, +, X
		vector<int[2]> localAttackSpots;
		
};

