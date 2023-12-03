// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#define LOG(x) std::cout << x << std::endl;

#include <stdlib.h> 
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Piece.h"


using namespace sf;



int boardSizeX = 8, boardSizeY = 8;

const std::string PAWN_WHITE = "pawn.png";
const std::string KNIGHT_WHITE = "knight.png";
const std::string BISHOP_WHITE = "bishop.png";
const std::string ROOK_WHITE = "rock.png";
const std::string QUEEN_WHITE = "queen.jpg";
const std::string KING_WHITE = "king.png";

const std::string PAWN_BLACK = "pawn.png";
const std::string KNIGHT_BLACK = "knight_black.png";
const std::string BISHOP_BLACK = "bishop_black.jpg";
const std::string ROOK_BLACK = "rook_black.png";
const std::string QUEEN_BLACK = "queen_black.jpg";
const std::string KING_BLACK = "king_black.png";

const Piece PAWN{ "pawn", PAWN_WHITE, PAWN_BLACK, ' ', { {0,1}, {0,2} } };
const Piece KING{ "king", KING_WHITE, KING_BLACK, ' ', { {0,1}, {1,0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1} } };
const Piece KNIGHT{ "knight", KNIGHT_WHITE, KNIGHT_BLACK, ' ', { {2,-1}, {2, 1}, {1,2 }, {-1, 2}, {-2, 1}, {-2, -1}, {-1,-2}, {1, -2} } };
const Piece BISHOP{ "bishop", BISHOP_WHITE, BISHOP_BLACK, 'X' ,{} };
const Piece QUEEN{ "queen", QUEEN_WHITE, QUEEN_BLACK, '*' ,{} };
const Piece ROOK{ "rook", ROOK_WHITE, ROOK_WHITE, '+' ,{} };

int chessboard[8][8] = {
    {-1, -2, -3, -4, -5, -3, -2, -1},
    {-6, -6, -6, -6, -6, -6, -6, -6},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 6,  6,  6,  6,  6,  6,  6,  6},
    { 1,  2,  3,  4,  5,  3,  2,  1}
};
bool currentTurn = true;

bool positionExist(int x2, int y2) {
    if (x2 > -1 and x2 < boardSizeX and y2 > -1 and y2 < boardSizeY)
        return true;
    return false;
}

int getPiece(Vector2i position) {
    if (positionExist(position.y, position.x))
       return chessboard[position.y][position.x];
    return 0;
}

Piece getPiece(int piece) {
    switch (piece) {
        case 1:
            return ROOK;
        case 2:
            return KNIGHT;
        case 3:
            return BISHOP;
        case 4:
            return QUEEN;
        case 5:
            return KING;
        case 6:
            return PAWN;
    }
}

bool canMove(Vector2i from, Vector2i to) {
    if (from == to)
        return false;

    int currPieceId = getPiece(from);
    int targetPieceId = getPiece(to);


    bool rightPiecesColors = currentTurn ? (currPieceId > 0 and targetPieceId <= 0) : (currPieceId < 0 and targetPieceId >= 0);


    if (rightPiecesColors) {
        //checking if piece can move to the square
        Piece piece = getPiece(abs(currPieceId));
        for (array<int, 2> pos : piece.localAttackSpots)
        {
            Vector2i globalAttackSpot = (currPieceId>0? Vector2i{ from.x - pos[0], from.y - pos[1]} : Vector2i{pos[0] + from.x, pos[1] + from.y});
            if (globalAttackSpot == to) {
                return true;
            }
            else
                LOG("X: " << globalAttackSpot.x << " Y: " << globalAttackSpot.y << " X: " << to.x << " Y: " << to.y);
        }
        return false;
    }
    else
        LOG((currentTurn ? "White turn" : "Black turn"));
    return false;
}

bool move(Vector2i from, Vector2i to) {
    if (canMove(from, to)) {
        int currPiece = getPiece(from);
        chessboard[from.y][from.x] = 0;
        chessboard[to.y][to.x] = currPiece;
        currentTurn = !currentTurn;
        return true;
    }
    return false;
}


int main()
{
    Color cellColor1(245, 222, 179);
    Color cellColor2(139, 69, 19);

    RenderWindow window(VideoMode(800, 800), "Chess");

    RectangleShape rectangle(Vector2f(100, 100));
    Vector2i selectedPos = Vector2i(-1, -1);
    Vector2i moveFrom = Vector2i(-1, -1);



   // LOG(checkPiece(Pawn));

    while (window.isOpen())
    {
        Vector2i pixelPos = Mouse::getPosition(window);
        Vector2i mousePos;
        mousePos = Vector2i(pixelPos.x / 100, pixelPos.y / 100);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonReleased)
                if (event.mouseButton.button == sf::Mouse::Left) {
                    selectedPos = mousePos;
                    if (moveFrom.x != -1 and moveFrom.y != -1) {
                        move(moveFrom, selectedPos);
                        moveFrom = Vector2i(-1, -1);
                        selectedPos = Vector2i(-1, -1);
                    }   
                    else if (getPiece(selectedPos) != 0)
                        moveFrom = selectedPos;
                }
                    
                
        }


        window.clear();
        for (int y = 0; y < 8; y++) 
        {
            for (int x = 0; x < 8; x++)
            {
                if (selectedPos.x == x and selectedPos.y == y) {
                    rectangle.setFillColor(Color::Red);
                }    
                else if (mousePos.x == x and mousePos.y == y)
                    rectangle.setFillColor(Color::Yellow);       
                else 
                    rectangle.setFillColor((x + y) % 2 == 0 ? cellColor1 : cellColor2);                  
                
                rectangle.setPosition(Vector2f(x * 100, y * 100));
                window.draw(rectangle);

                //getting piece image
                int pieceId = getPiece(Vector2i(x, y));
                Piece piece = getPiece(abs(pieceId));
                string pieceImgName = pieceId>0? piece.whiteSpriteName : piece.blackSpriteName;

                if (pieceImgName != "") {
                    Sprite pieceSprite;
                    Texture pieceTexture;
                    pieceTexture.loadFromFile(pieceImgName);
                    pieceSprite.setTexture(pieceTexture);
                    pieceSprite.setPosition(Vector2f(x * 100, y * 100));
                    window.draw(pieceSprite);
                } 
            }
        }
        window.display();
    }

    return 0;
}

