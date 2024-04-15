#include "Piece.h"

std::vector<possibleMove> Piece::CheckLine(std::array<std::array<int, 8>, 8> board, position pos, position direction, bool onlyAttack)
{
	std::vector<possibleMove> squaresUnderAttack;

	int currX = pos.x + direction.x;
	int currY = pos.y + direction.y;
	bool positionExists = currX < 8 and currX > -1 and currY < 8 and currY > -1;
	int currSquare;
	bool color = board[pos.y][pos.x] > 0;

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
		
		currX += direction.x;
		currY += direction.y;
	}
	return squaresUnderAttack;
}

Piece::Piece(std::string name, std::string whiteSpriteName, std::string blackSpriteName, char attackPattern, std::vector<position> localAttackSpots) {
	this->name = name;
	this->whiteSpriteName = whiteSpriteName;
	this->blackSpriteName = blackSpriteName;
	this->attackPattern = attackPattern;
	this->localAttackSpots = localAttackSpots;
}

std::vector<possibleMove> Piece::GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::array<std::array<int, 8>, 8> attackMap, std::string moves, position pos, bool onlyAttack) {
	
	std::vector<possibleMove> squaresUnderAttack = {};
	bool color = board[pos.y][pos.x] > 0;
	//checking localAttackSpots
	for (position attackSpot : localAttackSpots)
	{
		//defining actual(global/world) pos
		color? attackSpot = { pos.x - attackSpot.x, pos.y - attackSpot.y} : attackSpot = { pos.x + attackSpot.x, pos.y + attackSpot.y };
		//check if pos exists
		if (attackSpot.x > -1 and attackSpot.x < 8 and attackSpot.y > -1 and attackSpot.y < 8)
		{
			int piece = board[attackSpot.y][attackSpot.x];
			if (piece == 0 or (color ? piece < 0 : piece > 0) or onlyAttack)
				squaresUnderAttack.push_back({ attackSpot.x, attackSpot.y});
		}
	}
	//checking attack pattern
	if (attackPattern != ' ') {
		
		switch (attackPattern)
		{
		case 'X': {
			std::vector<possibleMove> topLeftLine = CheckLine(board, pos, { -1, 1 }, onlyAttack);
			std::vector<possibleMove> topRightLine = CheckLine(board, pos, { 1, 1 }, onlyAttack);
			std::vector<possibleMove> bottomLeftLine = CheckLine(board, pos, { -1, -1 }, onlyAttack);
			std::vector<possibleMove> bottomRightLine = CheckLine(board, pos, { 1, -1 }, onlyAttack);
			//squaresUnderAttack.reserve(topLeftLine.size() + topRightLine.size() + bottomLeftLine.size() + bottomRightLine.size());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topLeftLine.begin(), topLeftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topRightLine.begin(), topRightLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomLeftLine.begin(), bottomLeftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomRightLine.begin(), bottomRightLine.end());
			break;
		}	
		case '+': {
			std::vector<possibleMove> leftLine = CheckLine(board, pos, {  -1, 0  }, onlyAttack);
			std::vector<possibleMove> rightLine = CheckLine(board, pos, {  1, 0  }, onlyAttack);
			std::vector<possibleMove> topLine = CheckLine(board, pos, {  0, 1  }, onlyAttack);
			std::vector<possibleMove> bottomLine = CheckLine(board, pos, {  0, -1  }, onlyAttack);
			//squaresUnderAttack.reserve(leftLine.size() + rightLine.size() + bottomLine.size() + topLine.size());
			squaresUnderAttack.insert(squaresUnderAttack.end(), leftLine.begin(), leftLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), rightLine.begin(), rightLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), topLine.begin(), topLine.end());
			squaresUnderAttack.insert(squaresUnderAttack.end(), bottomLine.begin(), bottomLine.end());
			break;
		}
		case '*': {
			std::vector<possibleMove> leftLine = CheckLine(board, pos, {-1, 0 }, onlyAttack);
			std::vector<possibleMove> rightLine = CheckLine(board, pos, { 1, 0 }, onlyAttack);
			std::vector<possibleMove> topLine = CheckLine(board, pos, { 0, 1 }, onlyAttack);
			std::vector<possibleMove> bottomLine = CheckLine(board, pos, { 0, -1 }, onlyAttack);
			std::vector<possibleMove> topLeftLine = CheckLine(board, pos, { -1, 1 }, onlyAttack);
			std::vector<possibleMove> topRightLine = CheckLine(board, pos, { 1, 1 }, onlyAttack);
			std::vector<possibleMove> bottomLeftLine = CheckLine(board, pos, { -1, -1 }, onlyAttack);
			std::vector<possibleMove> bottomRightLine = CheckLine(board, pos, { 1, -1 }, onlyAttack);
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

