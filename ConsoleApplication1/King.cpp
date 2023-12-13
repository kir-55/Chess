#include "King.h"
#include <iostream>

King::King(std::string name, std::string whiteSpriteName, std::string blackSpriteName) : Piece(name, whiteSpriteName, blackSpriteName)
{
    
}

std::vector<possibleMove> King::GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::string moves, std::array<int, 2> position, bool onlyAttack)
{

    int x = position[0];
    int y = position[1];

    bool color = board[y][x] > 0;

    std::vector<possibleMove> squaresUnderAttack;

    std::vector<std::array<int, 2>> localAttackSpots = { {0,1}, {1,0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };

    //normal movement
    for (std::array<int, 2> attackSpot : localAttackSpots)
    {
        //defining actual(global/world) position
        color ? attackSpot = { position[0] - attackSpot[0], position[1] - attackSpot[1] } : attackSpot = { position[0] + attackSpot[0], position[1] + attackSpot[1] };
        //check if position exists
        if (attackSpot[0] > -1 and attackSpot[0] < 8 and attackSpot[1] > -1 and attackSpot[1] < 8)
        {
            int piece = board[attackSpot[1]][attackSpot[0]];
            if (piece == 0 or (color ? piece < 0 : piece > 0) or onlyAttack)
                squaresUnderAttack.push_back({ attackSpot[0], attackSpot[1] });
        }
    }
    if (!onlyAttack) {
        std::string kingMove = (color ? "e8" : "e1");
        std::string leftRookMove = (color ? "a8" : "a1");
        std::string rightRookMove = (color ? "h8" : "h1");
        std::cout << moves << "\n";


        if (moves.find(kingMove) == std::string::npos) {

            if (moves.find(leftRookMove) == std::string::npos) {

                bool freeSpace = board[position[1]][position[0] - 1] == 0 and
                    board[position[1]][position[0] - 2] == 0 and
                    board[position[1]][position[0] - 3] == 0;

                if (freeSpace) {
                    std::cout << (color ? "\nwhite" : "\nblack") << " left\n";
                    squaresUnderAttack.push_back({ position[0] - 2, position[1], CASTLE });
                }

            }
            if (moves.find(rightRookMove) == std::string::npos) {
                bool freeSpace = board[position[1]][position[0] + 1] == 0 and
                    board[position[1]][position[0] + 2] == 0;


                std::cout << (color ? "\nwhite" : "\nblack") << " +1: (" << board[position[1]][position[0] + 1] << "), +2: (" << board[position[1]][position[0] + 2] << ")";
                if (freeSpace) {
                    std::cout << (color ? "\nwhite" : "\nblack") << " right\n";
                    squaresUnderAttack.push_back({ position[0] + 2, position[1], CASTLE });
                }

            }
        }
    }
    


    return squaresUnderAttack;
}

void King::AfterMove(std::array<std::array<int, 8>, 8>& board, possibleMove move)
{
    if (move.moveType == CASTLE) {
        bool color = board[move.y][move.x] > 0;
        if (move.x == 2)
        {
            board[color ? 7 : 0][move.x + 1] = (color ? 1 : -1);
            board[color ? 7 : 0][move.x - 2] = 0;
        }
        else if (move.x == 6) {
            board[color ? 7 : 0][move.x - 1] = (color ? 1 : -1);
            board[color ? 7 : 0][move.x + 1] = 0;
        }
    }


}