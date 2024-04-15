#include "King.h"
#include <iostream>

King::King(std::string name, std::string whiteSpriteName, std::string blackSpriteName) : Piece(name, whiteSpriteName, blackSpriteName)
{
    
}

std::vector<possibleMove> King::GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::array<std::array<int, 8>, 8> attackMap, std::string moves, position pos, bool onlyAttack)
{

    int x = pos.x;
    int y = pos.y;

    bool color = board[y][x] > 0;

    std::vector<possibleMove> squaresUnderAttack;

    std::vector<position> localAttackSpots = { {0,1}, {1,0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };

    //normal movement
    for (position attackSpot : localAttackSpots)
    {
        //defining actual(global/world) pos
        color ? attackSpot = { pos.x - attackSpot.x, pos.y - attackSpot.y } : attackSpot = { pos.x + attackSpot.x, pos.y + attackSpot.y };
        //check if pos exists
        if (attackSpot.x > -1 and attackSpot.x < 8 and attackSpot.y > -1 and attackSpot.y < 8)
        {
            int piece = board[attackSpot.y][attackSpot.x];
            if (piece == 0 or (color ? piece < 0 : piece > 0) or onlyAttack)
                squaresUnderAttack.push_back({ attackSpot.x, attackSpot.y });
        }
    }
    if (!onlyAttack) {
        std::string kingMove = (color ? "e8" : "e1");
        std::string leftRookMove = (color ? "a8" : "a1");
        std::string rightRookMove = (color ? "h8" : "h1");
        std::cout << moves << "\n";


        if (moves.find(kingMove) == std::string::npos) {

            if (moves.find(leftRookMove) == std::string::npos) {

                bool freeSpace = board[pos.y][pos.x - 1] == 0 and
                                 board[pos.y][pos.x - 2] == 0 and
                                 board[pos.y][pos.x - 3] == 0;

                bool underAttack = attackMap[pos.y][pos.x] != 0 or
                                   attackMap[pos.y][pos.x - 1] != 0 or
                                   attackMap[pos.y][pos.x - 2] != 0;

                if (freeSpace and !underAttack) {
                    std::cout << (color ? "\nwhite" : "\nblack") << " left\n";
                    squaresUnderAttack.push_back({ pos.x - 2, pos.y, CASTLE });
                }

            }
            if (moves.find(rightRookMove) == std::string::npos) {
                bool freeSpace = board[pos.y][pos.x + 1] == 0 and
                    board[pos.y][pos.x + 2] == 0;

                bool underAttack = attackMap[pos.y][pos.x] != 0 or
                                   attackMap[pos.y][pos.x + 1] != 0 or
                                   attackMap[pos.y][pos.x + 2] != 0;


                std::cout << (color ? "\nwhite" : "\nblack") << " +1: (" << board[pos.y][pos.x + 1] << "), +2: (" << board[pos.y][pos.x + 2] << ")";
                if (freeSpace and !underAttack) {
                    std::cout << (color ? "\nwhite" : "\nblack") << " right\n";
                    squaresUnderAttack.push_back({ pos.x + 2, pos.y, CASTLE });
                }

            }
        }
    }
    


    return squaresUnderAttack;
}

void King::AfterMove(std::array<std::array<int, 8>, 8>& board, possibleMove move)
{
    if (move.moveType == CASTLE) {
        bool color = board[move.to.y][move.to.x] > 0;
        if (move.to.x == 2)
        {
            board[color ? 7 : 0][move.to.x + 1] = (color ? 5 : -5);
            board[color ? 7 : 0][move.to.x - 2] = 0;
        }
        else if (move.to.x == 6) {
            board[color ? 7 : 0][move.to.x - 1] = (color ? 5 : -5);
            board[color ? 7 : 0][move.to.x + 1] = 0;
        }
    }
}