﻿#define LOG(x) std::cout << x << std::endl;

#include <stdlib.h> 
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <enet/enet.h>
#include "Piece.h"
#include "Pawn.h"
#include "King.h"
#include "ChessConstants.h"
#include "Client.h"
#include <thread>

void loadMenu(tgui::BackendGui& gui, std::string message = "");

std::vector<possibleMove> possibleMoves;
std::array<std::array<int, 8>, 8> attackMap;
std::string movesNotation;

sf::Vector2i selectedPos = sf::Vector2i(-1, -1);
sf::Vector2i moveFrom = sf::Vector2i(-1, -1);

bool currentTurn = true;
bool playerColor = true;

bool firstDraw = true;
bool check = false;
int selectedPiece;
int squareSize = 100;

int gameMode = 0;
/* Game modes:
0 - menu
1 - local mode (1v1 on the same pc)
2 - PvP by network
*/ 

Client* client = nullptr;
std::thread receive;


tgui::Theme GUITheme{ CONTENT_PATH + "Theme.txt" };

sf::Color cellColor1(245, 222, 179);
sf::Color cellColor2(139, 69, 19);
sf::Color backgroundColor(67, 122, 55);
sf::Color GUIColor(86, 191, 82);

sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
tgui::Gui gui{ window };

std::array<std::array<int, 8>, 8> chessboard;
std::array<std::array<int, 8>, 8> initialChessboard{{
    {-1, -2, -3, -4, -5, -3, -2, -1},
    {-6, -6, -6, -6, -6, -6, -6, -6},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 6,  6,  6,  6,  6,  6,  6,  6},
    { 1,  2,  3,  4,  5,  3,  2,  1}}
};

sf::Vector2i getKingPos(bool color, std::array<std::array<int, 8>, 8> board = chessboard) {
    for(int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            if (board[y][x] == (color? 5 : -5))
                return sf::Vector2i(x, y);
   return sf::Vector2i(-1, -1);
}

bool positionExist(int x2, int y2) {
    if (x2 > -1 and x2 < boardSizeX and y2 > -1 and y2 < boardSizeY)
        return true;
    return false;
}

bool checkForCheck(sf::Vector2i kingPos, std::array<std::array<int, 8>, 8> attackMap) {

    std::cout << "king pos: " << kingPos.x << ", " << kingPos.y << "\n";
    if (kingPos.x != -1 and kingPos.y != -1) {
        if (attackMap[kingPos.y][kingPos.x] > 0) {
            std::cout << (currentTurn ? "White" : "Black") << " king is under attack\n";
            return true;
        }   
    }
    return false;
}

int getPiece(sf::Vector2i position) {
    if (positionExist(position.y, position.x))
        return chessboard[position.y][position.x];
    return 0;
}

Piece& getPiece(int piece) {
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

bool movePiece(sf::Vector2i from, sf::Vector2i to, std::array<std::array<int, 8>, 8>& board, std::vector<possibleMove> moves) {

    for (possibleMove possibleMove : moves) {
        if (possibleMove.x == to.x and possibleMove.y == to.y) {


            //local selected piece
            int sp = getPiece(from);

            board[from.y][from.x] = 0;
            board[to.y][to.x] = sp;
            std::string moveNote = std::string(1, abc[from.x]) + std::to_string(from.y + 1) + " " + std::string(1, abc[to.x]) + std::to_string(to.y + 1);

            /*std::cout << "\nto x: " << to.x << " to y: " << to.y << "\n";
            std::cout << "________\n";
            for (const auto& e : board) {
                for (const auto& e1 : e)
                    std::cout << e1 << "|";
                std::cout << "\n";
            }*/
            //std::cout << possibleMove.moveType << "\n";
            getPiece(abs(sp)).AfterMove(board, possibleMove);
            if (&board == &chessboard) {
                movesNotation += (" | " + moveNote);
                currentTurn = !currentTurn;

                if (gameMode == 1)
                    playerColor = currentTurn;
            }
            possibleMoves = {};
            return true;
        }
    }
    return false;
}

std::array<std::array<int, 8>, 8> getAttackMap(std::array<std::array<int, 8>, 8> board, bool color, std::string moves) {

    std::array<std::array<int, 8>, 8> attackMap = std::array<std::array<int, 8>, 8>();
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int currSquare = board[y][x];
            if (color ? currSquare < 0 : currSquare>0) {

                std::vector<possibleMove> attackedSquares = getPiece(abs(currSquare)).GetPossibleMoves(board, attackMap, moves, { {x,y} }, true);
                for (possibleMove possibleMove : attackedSquares) {
                    attackMap[possibleMove.y][possibleMove.x] += 1;
                }
            }
        }
    }
    return attackMap;

}

void drawSquare(sf::Vector2i pos, sf::Color color = sf::Color::White) {
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
    square.setPosition(sf::Vector2f(pos.x * squareSize, pos.y * squareSize));

    if (color == sf::Color::White)
        square.setFillColor((pos.x + pos.y % 2) ? cellColor1 : cellColor2);
    else
        square.setFillColor(color);

    window.draw(square);
}

void drawCircle(sf::Vector2i pos, sf::Color color = sf::Color::White) {
    sf::CircleShape circle;
    circle.setRadius(squareSize/4);
    circle.setFillColor(sf::Color(255, 255, 255, 0));
    circle.setOutlineThickness(squareSize / 5);
    circle.setOutlineColor(color);
    
    circle.setPosition(sf::Vector2f(pos.x * squareSize + squareSize/4, pos.y * squareSize + squareSize/4));

    if (color == sf::Color::White)
        circle.setFillColor((pos.x + pos.y % 2) ? cellColor1 : cellColor2);

    window.draw(circle);
}

void drawAttackMap(std::array<std::array<int, 8>, 8> attackedSquaresMap) {

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            int attackLevel = attackedSquaresMap[y][x];
            if (attackLevel > 0) {
                drawSquare(sf::Vector2i(x, y), sf::Color(attackLevel * 50, attackLevel * 50, attackLevel * 50));
            }
        }
}

std::vector<possibleMove> getSafeMoves(std::vector<possibleMove> moves, sf::Vector2i from) {
    std::vector<possibleMove> safeMoves;
    //std::cout << "--------\n";
    for (possibleMove move : moves) {
        std::array<std::array<int, 8>, 8> vrtBoard = chessboard;
        if (movePiece(from, sf::Vector2i(move.x, move.y), vrtBoard, moves)) {
            
            std::array<std::array<int, 8>, 8> vrtAttackMap = getAttackMap(vrtBoard, currentTurn, movesNotation);

            /*for (const auto& e : vrtAttackMap) {
                for (const auto& e1 : e)
                    std::cout << e1;
                std::cout << "\n";
            }*/
            //std::cout << "----------\n";

            //drawAttackMap(vrtAttackMap);

            if (!checkForCheck(getKingPos(currentTurn, vrtBoard), vrtAttackMap))
                safeMoves.push_back(move);
        }
        
    }
    return safeMoves;
}

void selectPiece(sf::Vector2i position) {
    int pieceId = getPiece(position);
    if (pieceId != 0 and currentTurn ? pieceId > 0:pieceId < 0) {
        selectedPiece = pieceId;
        std::array<int, 2> pos = std::array<int, 2>{ position.x, position.y };
        possibleMoves = getSafeMoves(getPiece(abs(pieceId)).GetPossibleMoves(chessboard, attackMap, movesNotation, pos), position);
        std::cout << "there is a piece!\n";
    }
    else {
        std::cout << "there is NO piece!\n";
        std::cout << "there is: " << pieceId;

        possibleMoves = {};
    }
        
}

bool checkIfCanMove() {
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++){
            int currSquare = chessboard[y][x];
            if (currentTurn ? currSquare > 0 : currSquare < 0) {
                    std::vector<possibleMove> pMoves = getSafeMoves(getPiece(abs(currSquare)).GetPossibleMoves(chessboard, attackMap, movesNotation, std::array<int, 2>{ x, y }), sf::Vector2i(x, y));
                    
                    //drawAttackMap(attackedSquaresMap);

                    //std::array<std::array<int, 8>, 8> currAttackMap = drawAttackMap(attackedSquaresMap);
                    //sf::Vector2i kingPos = getKingPos(currentTurn);
                    //checkForCheck(kingPos, currAttackMap);
                    
                    if (pMoves.size() > 0) {
                        std::cout << "\nthere is a move\n";

                        for (auto pMove : pMoves) {
                            std::cout << "___________\npiece: "<< currSquare << "\nx: " << pMove.x << "\ny : " << pMove.y << "\nmove type : " << pMove.moveType<< "\n \n";
                        }

                        for (const auto& e : chessboard) {
                            for (const auto& e1 : e)
                                std::cout << e1 << "|";
                            std::cout << "\n";
                        }

                        return true;
                    }
                        
            }
        }
    std::cout << "cannot move";
    return false;
}

void drawBoard() {

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++) {
            sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
            square.setPosition(sf::Vector2f(x * squareSize, y * squareSize));
            square.setFillColor(((x + y) % 2) ? cellColor1 : cellColor2);
            window.draw(square);
        }
}

void drawPieces() {
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
        {
            int pieceId = getPiece(sf::Vector2i(x, y));
            if (pieceId == 0)
                continue;
            const Piece piece = getPiece(abs(pieceId));
            std::string pieceImgName = pieceId > 0 ? piece.whiteSpriteName : piece.blackSpriteName;

            if (pieceImgName != "") {
                sf::Sprite pieceSprite;
                sf::Texture pieceTexture;
                float scale = 0.007 * squareSize;


                pieceTexture.loadFromFile(pieceImgName);
                pieceSprite.setTexture(pieceTexture);
                pieceSprite.setScale(scale, scale);

                int posX = (playerColor? x : 7 - x) * squareSize + (squareSize - pieceSprite.getLocalBounds().width * scale) / 2;
                int posY = (playerColor? y : 7 - y) * squareSize + (squareSize - pieceSprite.getLocalBounds().height * scale) / 2;
                pieceSprite.setPosition(sf::Vector2f(posX, posY));

                window.draw(pieceSprite);
            }
        }
}

void initializeOnline(const char username[]) {
    client = new Client(username);

    if (client->ready)
    {
        std::cout << "\nconnected!!\n";
        receive = std::thread(&Client::ReceiveLoop, client);
        bool color = client->GetColor();
        playerColor = color;
        currentTurn = true;
        std::cout << "Color: " << color << "\n";
    }
    else {
        std::cout << "connection failed!!\n";
        gameMode = 0;
        loadMenu(gui, "connection failed!!");
    }
    //std::thread receive(client{client.receiveloop();});

}

void sendMove(sf::Vector2i from, sf::Vector2i to) {
    std::string x = "0|" + std::to_string((from.x+1)*1000 + (from.y+1)*100 + (to.x+1)*10 + to.y + 1);
    const char* data = x.c_str();
    /// <summary>
    /// digits:
    /// 1 - from x
    /// 2 - from y
    /// 3 - to x
    /// 4 - to y
    /// </summary>
    std::cout << "sent move(string): " << x << "\n";
    std::cout << "sent move(char): " << data << "\n";
    client->SendPacket(data);
}

void play(tgui::EditBox::Ptr username, int mode)
{
    tgui::String u_name = username->getText();
    if (u_name != "") {
        gui.removeAllWidgets();
        std::cout << "logged as: " << u_name;
        gameMode = mode;
        chessboard = initialChessboard;
        currentTurn = true;
        playerColor = true;
        possibleMoves = {};
        movesNotation = "";
        check = false;
        
        //std::basic_string<char32_t> name = u_name.c_str();
        //char username_L[80];
        //strcpy(username_L ,  u_name.c_str());
        
        if (gameMode == 2) {
            // Convert tgui::String to std::string, then to const char*
            std::string u_name_str;
            for (std::size_t i = 0; i < u_name.length(); ++i) {
                u_name_str += static_cast<char>(u_name[i]);
            }
            const char* u_name_cstr = u_name_str.c_str();
            initializeOnline(u_name_cstr);
        }
            ///error uname is type of tgui::string and i need const char *
    }
}

void updateTextSize(tgui::BackendGui& gui)
{
    // Update the text size of all widgets in the gui, based on the current window height
    const float windowHeight = gui.getView().getRect().height;
    gui.setTextSize(static_cast<unsigned int>(0.07f * windowHeight)); // 7% of height
}

void loadMenu(tgui::BackendGui& gui, std::string message)
{
    updateTextSize(gui);

    // We want the text size to be updated when the window is resized
    gui.onViewChange([&gui] { updateTextSize(gui); });
    if (message != "") {
        auto label = tgui::Label::create();
        label->setText(message);
        label->setPosition({ "28%", "10%" });
        label->setTextSize(24);
        label->setRenderer(GUITheme.getRenderer("Label"));
        gui.add(label);
    }
    

    auto editBoxUsername = tgui::EditBox::create();
    editBoxUsername->setSize({ "66.67%", "12.5%" });
    editBoxUsername->setPosition({ "16.67%", "30%" });
    editBoxUsername->setDefaultText("Nickname");
    editBoxUsername->setMaximumCharacters(80);
    editBoxUsername->setRenderer(GUITheme.getRenderer("EditBox"));
    gui.add(editBoxUsername);

    auto playOffline = tgui::Button::create("Play offline");
    playOffline->setSize({ "50%", "16.67%" });
    playOffline->setPosition({ "25%", "50%" });
    playOffline->setRenderer(GUITheme.getRenderer("Button"));
    gui.add(playOffline);

    auto playOnline = tgui::Button::create("Play online");
    playOnline->setSize({ "50%", "16.67%" });
    playOnline->setPosition({ "25%", "70%" });
    playOnline->setRenderer(GUITheme.getRenderer("Button"));
    gui.add(playOnline);

    // Call the login function when the button is pressed and pass the edit boxes that we created as parameters
    // The "&" in front of "login" can be removed on newer compilers, but is kept here for compatibility with GCC < 8.
    playOffline->onPress(&play, editBoxUsername, 1);
    playOnline->onPress(&play, editBoxUsername, 2);
}

bool processStep(sf::Vector2i from, sf::Vector2i to) {
    if (movePiece(from, to, chessboard, possibleMoves)) {
        attackMap = getAttackMap(chessboard, currentTurn, movesNotation);
        sf::Vector2i kingPos = getKingPos(currentTurn, chessboard);

        if (checkForCheck(kingPos, attackMap)) {
            check = true;
        }
        else {
            check = false;
        }

        if (gameMode == 2)
            sendMove(from, to);

        if (!checkIfCanMove()) {
            if (check) {
                std::cout << "Checkmate!\n" << (currentTurn ? "Black" : "White") << " won.\n";
                gameMode = 0;
                std::string Winner = currentTurn ? "Black" : "White";
                delete client;
                client = nullptr;

                loadMenu(gui, "Checkmate!\n" + Winner + " won.");
            }
            else {
                std::cout << "Stalemate!\n Its draw.";
                gameMode = 0;
                delete client;
                client = nullptr;
                loadMenu(gui, "Stalemate!\n Its draw.");
            }
        }



        return true;
    }
    return false;
}

void draw() {
    window.clear();

    drawBoard();

    // Attack map & visualisation
    //std::array<std::array<int, 8>, 8> attackedSquaresMap = getAttackMap(chessboard, currentTurn, movesNotation);
    //drawAttackMap(attackedSquaresMap);

    sf::Vector2i kingPos = getKingPos(currentTurn, chessboard);
    if (check) {
        sf::Vector2i position = playerColor ? kingPos : sf::Vector2i(7 - kingPos.x, 7 - kingPos.y);
        drawSquare(position, sf::Color(191, 114, 114, 150));
    }

    if (selectedPos != sf::Vector2i(-1, -1)) {
        sf::Vector2i position = playerColor ? selectedPos : sf::Vector2i(7 - selectedPos.x, 7 - selectedPos.y);
        drawSquare(position, sf::Color(114, 191, 137, 100));
    }


    for (possibleMove possibleMove : possibleMoves) {
        sf::Vector2i position = playerColor ? sf::Vector2i(possibleMove.x, possibleMove.y)
            : sf::Vector2i(7 - possibleMove.x, 7 - possibleMove.y);
        drawCircle(position, sf::Color(133, 133, 133, 100));
    }

    drawPieces();
    window.display();
    
}

int main()
{
    while (window.isOpen())
    {
        if (firstDraw) {
            if (gameMode != 0) {
                draw();
            }
            else {
                loadMenu(gui);
                window.clear(backgroundColor);
                gui.draw();
                window.display();
            }

            firstDraw = false;
        }

        sf::Event event;
        while ((client != nullptr and client->lastMove != 0) or window.waitEvent(event))
        {
            if (gameMode == 2 and (client != nullptr and client->lastMove != 0)) {

                sf::Vector2i from((client->lastMove / 1000) - 1, (client->lastMove / 100 % 10) - 1);
                sf::Vector2i to((client->lastMove / 10 % 10) - 1, (client->lastMove % 10) - 1);

                selectPiece(from);
                if (processStep(from, to)) {
                    draw();
                    std::cout << "success\n";
                }
                else {
                    std::cout << "no success\n";
                }
                if(client != nullptr)
                    client->lastMove = 0;
            }



            gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized) {
                int h = event.size.height;
                int w = event.size.width;

                if (w < h)
                    squareSize = w / 8;
                else
                    squareSize = h / 8;


                window.setSize(sf::Vector2u(squareSize * 8, squareSize * 8));

                sf::FloatRect visibleArea(0, 0, squareSize * 8, squareSize * 8);
                window.setView(sf::View(visibleArea));

                selectedPos = sf::Vector2i(-1, -1);
                moveFrom = sf::Vector2i(-1, -1);

                if (gameMode != 0) {
                    drawBoard();
                    drawPieces();
                }
                else {
                    window.clear(backgroundColor);
                    gui.draw();
                }

                window.display();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (gameMode == 0) {
                    window.clear(backgroundColor);
                    gui.draw();
                }
                    
                window.display();
            }
            else if (event.type == sf::Event::MouseButtonReleased and event.mouseButton.button == sf::Mouse::Left                             ) {
                std::cout << "gameMode: " << gameMode << "\n";
                if (gameMode != 0) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2i mousePos;

                    if(playerColor)
                        mousePos = sf::Vector2i(pixelPos.x / squareSize, pixelPos.y / squareSize);
                    else
                        mousePos = sf::Vector2i(7-pixelPos.x / squareSize, 7-pixelPos.y / squareSize);

                    if (selectedPos != mousePos) {
                        selectedPos = mousePos;

                        if (playerColor == currentTurn) {
                            if (moveFrom.x != -1 and moveFrom.y != -1) {
                                if (processStep(moveFrom, selectedPos)) {
                                    selectedPos = sf::Vector2i(-1, -1);
                                    moveFrom = sf::Vector2i(-1, -1);
                                }
                                else {
                                    possibleMoves = {};
                                    if (playerColor ? (getPiece(selectedPos) > 0) : (getPiece(selectedPos) < 0))
                                    {
                                        selectPiece(selectedPos);
                                        moveFrom = selectedPos;
                                    }
                                    else {
                                        moveFrom = sf::Vector2i(-1, -1);
                                        selectedPos = sf::Vector2i(-1, -1);
                                    }
                                }
                            }
                            else if (getPiece(selectedPos) != 0) {
                                selectPiece(selectedPos);
                                moveFrom = selectedPos;
                            }
                            else
                                selectedPos = sf::Vector2i(-1, -1);
                        }
                            

                        draw();
                    }
                }

                if (gameMode == 0) {
                    if (receive.joinable())
                        receive.join();

                    window.clear(backgroundColor);
                    gui.draw();
                    window.display();
                }

                if (firstDraw)
                    firstDraw = false;
            }

        }


    }

    if (receive.joinable())
        receive.join();

    return 0;
}


