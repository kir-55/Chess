#ifndef BOT_H
#define BOT_H

#include "ChessConstants.h";

class Bot
{

	private:
		std::string m_name;
	public: 
		Bot(std::string name, int depth = 4);
		int depth = 4;
		float Evaluate(std::array<std::array<int, 8>, 8> board, std::string movesNotation, bool color);
		virtual globalMove FindMove(std::array<std::array<int, 8>, 8> board, std::string movesNotation, bool color);
		float minimax(int depth, float alpha, float beta, bool maximizingPlayer, std::array<std::array<int, 8>, 8> board, std::string movesNotation);
};

#endif