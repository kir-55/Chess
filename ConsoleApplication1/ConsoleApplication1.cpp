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
const std::string KNIGHT_WHITE = "";
const std::string BISHOP_WHITE = "";
const std::string ROOK_WHITE = "";
const std::string QUEEN_WHITE = "";
const std::string KING_WHITE = "";

const std::string PAWN_BLACK = "pawn.png";
const std::string KNIGHT_BLACK = "";
const std::string BISHOP_BLACK = "";
const std::string ROOK_BLACK = "";
const std::string QUEEN_BLACK = "";
const std::string KING_BLACK = "";



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

bool positionExist(int x2, int y2) {
    if (x2 > -1 and x2 < boardSizeX and y2 > -1 and y2 < boardSizeY)
        return true;
    return false;
}

std::string getPieceImageName(int x1, int y1) {
    if (positionExist(x1, y1) and chessboard[y1][x1] != 0) {
        int currPiece = chessboard[y1][x1];
        switch (currPiece) {
            case 6:
                return PAWN_WHITE;
            case -6:
                return PAWN_BLACK;
            default:
                return "";
        }
    }
    return "";
}
std::vector<Vector2i> highlightPawnMoves(Vector2i position, bool isWhite) {
    std::vector<Vector2i> validMoves;

    int direction = isWhite ? -1 : 1;

    // Forward move
    int newY = position.y + direction;
    if (positionExist(position.x, newY) && chessboard[newY][position.x] == 0) {
        validMoves.emplace_back(position.x, newY);
    }

    // Diagonal captures
    int newX1 = position.x - 1;
    int newX2 = position.x + 1;

    if (positionExist(newX1, newY) && chessboard[newY][newX1] * direction < 0) {
        validMoves.emplace_back(newX1, newY);
    }

    if (positionExist(newX2, newY) && chessboard[newY][newX2] * direction < 0) {
        validMoves.emplace_back(newX2, newY);
    }

    return validMoves;
}



int main()
{
    Color cellColor1(245, 222, 179);
    Color cellColor2(139, 69, 19);

    RenderWindow window(VideoMode(800, 800), "SFML works!");

    RectangleShape rectangle(Vector2f(100, 100));
    Vector2i selectedPos;

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
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    selectedPos = mousePos;
                    std::cout << "help";
                }
            }
        }


        window.clear();
        for (int y = 0; y < 8; y++) 
        {
            for (int x = 0; x < 8; x++)
            {
                if (selectedPos.x == x and selectedPos.y == y) {
                    rectangle.setFillColor(Color::Red);
                    std::vector<Vector2i> validMoves = highlightPawnMoves(selectedPos, true);

                    for (const Vector2i& move : validMoves) {
                        int moveX = move.x;
                        int moveY = move.y;
                        rectangle.setFillColor(Color::Green);
                        rectangle.setPosition(Vector2f(moveX * 100, moveY * 100));
                        window.draw(rectangle);
                    }
                }    
                else if (mousePos.x == x and mousePos.y == y)
                    rectangle.setFillColor(Color::Yellow);       
                else 
                    rectangle.setFillColor((x + y) % 2 == 0 ? cellColor1 : cellColor2);                  
                
                rectangle.setPosition(Vector2f(x * 100, y * 100));
                window.draw(rectangle);

                std::string pieceImgName = getPieceImageName(x, y);

                if (pieceImgName != "") {
                    Sprite pieceSprite;
                    Texture pieceTexture;
                    pieceTexture.loadFromFile(pieceImgName);
                    pieceSprite.setTexture(pieceTexture);
                    pieceSprite.setPosition(Vector2f(x * 100, y * 100));
                    window.draw(pieceSprite);
                }
                else{
                    sf::Font font;
                    if(font.loadFromFile("arial.ttf"))
                    {
                        Text text;
                        text.setFont(font);
                        text.setPosition(Vector2f(x * 100, y * 100));
                        text.setString("none");
                        window.draw(text);
                    }
                }
                
            }
        }
        window.display();
    }

    return 0;
}

