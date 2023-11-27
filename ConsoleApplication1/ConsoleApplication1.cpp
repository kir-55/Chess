// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <stdlib.h> 
#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;

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


int main()
{
    Color cellColor1(245, 222, 179);
    Color cellColor2(139, 69, 19);

    RenderWindow window(VideoMode(800, 800), "SFML works!");

    RectangleShape rectangle(Vector2f(100, 100));
    Texture texture;
    texture.loadFromFile("pawn.png");
    Sprite sprite(texture);




    

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        for (int y = 0; y < 8; y++) 
        {
            for (int x = 0; x < 8; x++)
            {
                rectangle.setFillColor((x+y)%2 == 0?cellColor1: cellColor2);
                rectangle.setPosition(Vector2f(x*100, y*100));
                window.draw(rectangle);
            }
        }
        
        window.draw(sprite);
        window.display();
    }

    return 0;
}

