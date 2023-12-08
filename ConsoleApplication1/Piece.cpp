#include "Piece.h"
#include <iostream>

Piece::Piece(string name, string whiteSpriteName, string blackSpriteName, char attackPattern, vector<array<int, 2>> localAttackSpots) {
	this->name = name;
	this->whiteSpriteName = whiteSpriteName;
	this->blackSpriteName = blackSpriteName;
	this->attackPattern = attackPattern;
	this->localAttackSpots = localAttackSpots;
}

vector<array<int, 2>> Piece::getAttackedSquares(array<array<int, 8>, 8> board, array<int, 2> position) {
	
	vector<array<int, 2>> squareUnderAttack;
	bool color = board[position[0]][position[0]] > 0;
	for (array<int, 2> attackSpot : localAttackSpots)
	{
		color? attackSpot = { position[0] - attackSpot[0], position[1] - attackSpot[1]} : attackSpot = { position[0] + attackSpot[0], position[1] + attackSpot[1] };
		if (attackSpot[0] < 0 or attackSpot[0] > 7 or attackSpot[1] < 0 or attackSpot[1] > 7)
			cout << "xd@";
		else {
			int piece = board[attackSpot[0]][attackSpot[1]];

			if (piece == 0 or color ? piece < 0 : piece > 0)
				squareUnderAttack.push_back(attackSpot);
			std::cout << "\nxd2";
		}

		

	}
	return squareUnderAttack;
}