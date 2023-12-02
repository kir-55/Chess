// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <stdlib.h> 
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>



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

std::string getPieceImageName(int piece) {
    switch (piece) {
        case 1:
            return ROOK_WHITE;
        case 2:
            return KNIGHT_WHITE;
        case 3:
            return BISHOP_WHITE;
        case 4:
            return QUEEN_WHITE;
        case 5:
            return KING_WHITE;
        case 6:
            return PAWN_WHITE;
        case -1:
            return ROOK_BLACK;
        case -2:
            return KNIGHT_BLACK;
        case -3:
            return BISHOP_BLACK;
        case -4:
            return QUEEN_BLACK;
        case -5:
            return KING_BLACK;
        case -6:
            return PAWN_BLACK;
        default:
            return "";
    }
}

bool move(Vector2i from, Vector2i to) {
    int currPiece = getPiece(from);
    int targetPiece = getPiece(to);

    std::cout << currPiece;
    if (from != to
        and (currentTurn? (currPiece > 0) : (currPiece < 0))
        and (targetPiece == 0 or (currentTurn? (targetPiece < 0) : (targetPiece > 0))) ){
        chessboard[from.y][from.x] = 0;
        chessboard[to.y][to.x] = currPiece;
        currentTurn = !currentTurn;
        return true;
    }
    else
        std::cout << std::endl << (currentTurn? "white turn" : "black turn") << std::endl;
    return false;
}


int main()
{
    Color cellColor1(245, 222, 179);
    Color cellColor2(139, 69, 19);

    RenderWindow window(VideoMode(800, 800), "SFML works!");

    RectangleShape rectangle(Vector2f(100, 100));
    Vector2i selectedPos = Vector2i(-1, -1);
    Vector2i moveFrom = Vector2i(-1, -1);

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

                std::string pieceImgName = getPieceImageName(getPiece(Vector2i(x,y)));

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

