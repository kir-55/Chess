#include "Piece.h"

std::vector<possibleMove> Piece::CheckLine(std::array<std::array<int, 8>, 8> board, std::array<int, 2> position, std::array<int, 2> direction, bool onlyAttack)
{
	std::vector<possibleMove> squaresUnderAttack;

	int currX = position[0] + direction[0];
	int currY = position[1] + direction[1];
	bool positionExists = currX < 8 and currX > -1 and currY < 8 and currY > -1;
	int currSquare;
	bool color = board[position[1]][position[0]] > 0;

	while (positionExists) {
		positionExists = currX < 8 and currX > -1 and currY < 8 and currY > -1;
		if (!positionExists)
			break;
		currSquare = board[currY][currX];
		if (!currSquare)
			squaresUnderAttack.push_back({ currX, currY, MOVE });
		else if ((color ? (currSquare < 0) : (currSquare > 0)) or onlyAttack){
			squaresUnderAttack.push_back({ currX, currY, CAPTURE });
			break;
		}
		else 
			break;
		
		currX += direction[0];
		currY += direction[1];
	}
	return squaresUnderAttack;
}

Piece::Piece(std::string name, std::string whiteSpriteName, std::string blackSpriteName, char attackPattern, std::vector<std::array<int, 2>> localAttackSpots) {
	this->name = name;
	this->whiteSpriteName = whiteSpriteName;
	this->blackSpriteName = blackSpriteName;
	this->attackPattern = attackPattern;
	this->localAttackSpots = localAttackSpots;
}

std::vector<possibleMove> Piece::GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::array<std::array<int, 8>, 8> attackMap, std::string moves, std::array<int, 2> position, bool onlyAttack) {
	
	std::vector<possibleMove> squaresUnderAttack;
	bool color = board[position[1]][position[0]] > 0;
	//checking localAttackSpots
	for (std::array<int, 2> attackSpot : localAttackSpots)
	{
		//defining actual(global/world) position
		color? attackSpot = { position[0] - attackSpot[0], position[1] - attackSpot[1]} : attackSpot = { position[0] + attackSpot[0], position[1] + attackSpot[1] };
		//check if position exists
		if (attackSpot[0] > -1 and attackSpot[0] < 8 and attackSpot[1] > -1 and attackSpot[1] < 8)
		{
			int piece = board[attackSpot[1]][attackSpot[0]];
			if (piece == 0 or (color ? piece < 0 : piece > 0) or onlyAttack)
				squaresUnderAttack.push_back({ attackSpot[0], attackSpot[1]});
		}
	}
	//checking attack pattern
	if (attackPattern != ' ') {
		
		switch (attackPattern)
		{
		case 'X': {
			std::vector<possibleMove> topLeftLine = CheckLine(board, position, { { -1, 1 } }, onlyAttack);
			std::vector<possibleMove> topRightLine = CheckLine(board, position, { { 1, 1 } }, onlyAttack);
			std::vector<possibleMove> bottomLeftLine = CheckLine(board, position, { { -1, -1 } }, onlyAttack);
			std::vector<possibleMove> bottomRightLine = CheckLine(board, position, { { 1, -1 } }, onlyAttack);
			//squaresUnderAttack.reserve(topLeftLine.size() + topRightLine.size() + bottomLeftLine.size() + bottomRightLine.size());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topLeftLine.begin(), topLeftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topRightLine.begin(), topRightLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomLeftLine.begin(), bottomLeftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomRightLine.begin(), bottomRightLine.end());
			break;
		}	
		case '+': {
			std::vector<possibleMove> leftLine = CheckLine(board, position, { { -1, 0 } }, onlyAttack);
			std::vector<possibleMove> rightLine = CheckLine(board, position, { { 1, 0 } }, onlyAttack);
			std::vector<possibleMove> topLine = CheckLine(board, position, { { 0, 1 } }, onlyAttack);
			std::vector<possibleMove> bottomLine = CheckLine(board, position, { { 0, -1 } }, onlyAttack);
			//squaresUnderAttack.reserve(leftLine.size() + rightLine.size() + bottomLine.size() + topLine.size());
			squaresUnderAttack.insert(squaresUnderAttack.end(), leftLine.begin(), leftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), rightLine.begin(), rightLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topLine.begin(), topLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomLine.begin(), bottomLine.end());
			break;
		}
		case '*': {
			std::vector<possibleMove> leftLine = CheckLine(board, position, { { -1, 0 } }, onlyAttack);
			std::vector<possibleMove> rightLine = CheckLine(board, position, { { 1, 0 } }, onlyAttack);
			std::vector<possibleMove> topLine = CheckLine(board, position, { { 0, 1 } }, onlyAttack);
			std::vector<possibleMove> bottomLine = CheckLine(board, position, { { 0, -1 } }, onlyAttack);
			std::vector<possibleMove> topLeftLine = CheckLine(board, position, { { -1, 1 } }, onlyAttack);
			std::vector<possibleMove> topRightLine = CheckLine(board, position, { { 1, 1 } }, onlyAttack);
			std::vector<possibleMove> bottomLeftLine = CheckLine(board, position, { { -1, -1 } }, onlyAttack);
			std::vector<possibleMove> bottomRightLine = CheckLine(board, position, { { 1, -1 } }, onlyAttack);
			//squaresUnderAttack.reserve(topLeftLine.size() + topRightLine.size() + bottomLeftLine.size() + bottomRightLine.size());
			squaresUnderAttack.insert(squaresUnderAttack.end(), leftLine.begin(), leftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), rightLine.begin(), rightLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topLine.begin(), topLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomLine.begin(), bottomLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topLeftLine.begin(), topLeftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topRightLine.begin(), topRightLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomLeftLine.begin(), bottomLeftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomRightLine.begin(), bottomRightLine.end());
			break;
		}
		default:
			break;
		}
	}
		
	
	return squaresUnderAttack;
}

void Piece::AfterMove(std::array<std::array<int, 8>, 8>& board, possibleMove move)
{
	//nothing here
}

