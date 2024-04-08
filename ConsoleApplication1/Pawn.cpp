#include "Pawn.h"



Pawn::Pawn(std::string name, std::string whiteSpriteName, std::string blackSpriteName) : Piece(name, whiteSpriteName, blackSpriteName){}

std::vector<possibleMove> Pawn::GetPossibleMoves(std::array<std::array<int, 8>, 8> board, std::array<std::array<int, 8>, 8> attackMap, std::string moves, std::array<int, 2> position, bool onlyAttack)
{    

    int x = position[0];
    int y = position[1];

    bool color = board[y][x] > 0;

    std::vector<possibleMove> squareUnderAttack;


    
    //if have free space in front
    if(color ? (y - 1 > -1) : (y + 1 < 8) )
    {
        
            
        //right attack move
        if (x + 1 < 8) {
            int attackRightSquare = board[y + (color ? -1 : 1)][x + 1];
            if ((attackRightSquare and color ? attackRightSquare < 0 : attackRightSquare > 0) or onlyAttack) {
                if (y + (color ? -1 : 1) == (color ? 0 : 7)) {
                    squareUnderAttack.push_back({ x + 1, y + (color ? -1 : 1), PROMOTION });
                }
                else {
                    squareUnderAttack.push_back({ x + 1, y + (color ? -1 : 1), CAPTURE });
                }
            }
                
        }
        //left attack move
        if (x - 1 > -1) {
            int attackLeftSquare = board[y + (color ? -1 : 1)][x - 1];
            if ((attackLeftSquare and color ? attackLeftSquare < 0 : attackLeftSquare > 0) or onlyAttack) {
                if (y + (color ? -1 : 1) == (color ? 0 : 7)) {
                    squareUnderAttack.push_back({ x - 1, y + (color ? -1 : 1), PROMOTION });
                }
                else {
                    squareUnderAttack.push_back({ x - 1, y + (color ? -1 : 1), CAPTURE });
                }
            }
             
        }

        //single move
        int moveSquare = board[y + (color ? -1 : 1)][x];
        if (!moveSquare and !onlyAttack) {
            if(y + (color ? -1:1) == (color ? 0 : 7))
                squareUnderAttack.push_back({ x, y + (color ? -1 : 1), PROMOTION });
            else
                squareUnderAttack.push_back({ x, y + (color ? -1 : 1), MOVE });
            //double move
            if (color ? (y - 2 > -1) : (y + 2 < 8)) {
                int doubleMoveSquare = board[y + (color ? -2 : 2)][x];
                if ((color ? y == 6 : y == 1) and !doubleMoveSquare)
                    squareUnderAttack.push_back({ x, y + (color ? -2 : 2), MOVE });
            }
        }
        
        if(onlyAttack)
            return squareUnderAttack;

    }
    const std::array<char, 8> abc = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };//??
    //if have free space on right side
    if (x + 1 < 8) {
        //En passant right
        int enPassantRight = board[y][x + 1];
        //if last opponent move was pawn move
        if (enPassantRight == color ? -1 : 1) {
            std::string lastMove = moves.rfind(" | ") != -1 ? moves.substr(moves.rfind(" | "), moves.size()) : moves;
            if (lastMove.size() >= 3)
                lastMove = lastMove.replace(0, 3, "");

            
            std::string xNotation = std::string(1, abc[x + 1]);
            if (lastMove == xNotation + std::to_string(y + 1 + (color ? -2 : 2)) + " " + xNotation + std::to_string(y + 1)) {
                squareUnderAttack.push_back({ x + 1, y + (color ? -1 : 1), EN_PASSANT });
                board[y][x + 1] = 0;
            }

            //std::cout << "lm: " + lastMove + "\n";
            //std::cout << "en: " + xNotation + std::to_string(y + 1 + (color ? -2 : 2)) + " " + xNotation + std::to_string(y + 1 ) + "\n";
        }
    }
    //if have free space on left side
    if (x - 1 > -1) { 
        int enPassantLeft = board[y][x - 1];
        if (enPassantLeft == color ? -1 : 1) {
            std::string lastMove = moves.rfind(" | ") != -1 ? moves.substr(moves.rfind(" | "), moves.size()) : moves;
            if (lastMove.size() >= 3)
                lastMove = lastMove.replace(0, 3, "");

            std::string xNotation = std::string(1, abc[x - 1]);
            if (lastMove == xNotation + std::to_string(y + 1 + (color ? -2 : 2)) + " " + xNotation + std::to_string(y + 1)) {
                squareUnderAttack.push_back({ x - 1, y + (color ? -1 : 1), EN_PASSANT });
            }
                
            //std::cout << "en: " + xNotation + std::to_string(y + 1 + (color ? -2 : 2)) + " " + xNotation + std::to_string(y + 1) + "\n";
        }
    }
    

    return squareUnderAttack;
}

void Pawn::AfterMove(std::array<std::array<int, 8>, 8>& board, possibleMove move)
{
    if (move.moveType == EN_PASSANT) {
        bool color = board[move.y][move.x] > 0;
        board[move.y + (color ? 1 : -1)][move.x] = 0;
        std::cout << "En Passant" << "\n";
    }
    if (move.moveType == PROMOTION) {
        bool color = board[move.y][move.x] > 0;
        board[move.y][move.x] = color? 9 : -9;
        std::cout << "Pawn Promotion" << "\n";
    }

}
