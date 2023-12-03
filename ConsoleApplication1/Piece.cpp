#include "Piece.h"

Piece::Piece(string name, string whiteSpriteName, string blackSpriteName, char attackPattern, vector<array<int, 2>> localAttackSpots) {
	this->name = name;
	this->whiteSpriteName = whiteSpriteName;
	this->blackSpriteName = blackSpriteName;
	this->attackPattern = attackPattern;
	this->localAttackSpots = localAttackSpots;
}