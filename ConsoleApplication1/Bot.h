#ifndef BOT_H
#define BOT_H

#include "ChessConstants.h";

class Bot
{

	private:
		std::string m_name;
	public: 
		int layers = 2;
		virtual float Evaluate();
		virtual globalMove FindMove();
		Bot(std::string name, int levels = 2);
};

#endif