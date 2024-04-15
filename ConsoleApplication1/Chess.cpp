#include <stdlib.h> 
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <enet/enet.h>
#include "ChessConstants.h"
#include "ChessMethods.h"
#include "Moves.h"
#include "Client.h"
#include "Bot.h"

void loadMenu(tgui::BackendGui& gui, std::string message = "");

globalMove lastMove = {};


std::vector<possibleMove> possibleMoves;
std::array<std::array<int, 8>, 8> attackMap, botGlobalAttackMap;
std::string movesNotation;

sf::Vector2i selectedPos = sf::Vector2i(-1, -1);
sf::Vector2i moveFrom = sf::Vector2i(-1, -1);

bool currentTurn = true;
bool playerColor = false;

bool firstDraw = true;
bool check = false;
bool developerMode = false;
int selectedPiece;
int squareSize = 100;

int gameMode = 0;
/* Game modes:
0 - menu
1 - local mode (1v1 on the same pc)
2 - PvP by network
3 - with bot
*/ 

Client* client = nullptr;
std::thread receive;

Bot Ridrisch = Bot("ridrisch");

tgui::Theme GUITheme{ CONTENT_PATH + "Theme.txt" };

sf::Color cellColor1(245, 222, 179);
sf::Color cellColor2(139, 69, 19);
sf::Color backgroundColor(67, 122, 55);
sf::Color GUIColor(86, 191, 82);

sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");
tgui::Gui gui{ window };

std::array<std::array<int, 8>, 8> chessboard;
std::array<std::array<int, 8>, 8> initialChessboard{{
    {-5, -3, -4, -9, -10, -4, -3, -5},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1},
    { 5,  3,  4,  9,  10,  4,  3,  5}}
};

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

void selectPiece(position pos) {
    int pieceId = getPiece(pos, chessboard);
    if (pieceId != 0 and currentTurn ? pieceId > 0:pieceId < 0) {
        selectedPiece = pieceId;
        possibleMoves = getSafeMoves(chessboard, getPiece(abs(pieceId)).GetPossibleMoves(chessboard, attackMap, movesNotation, pos), pos, currentTurn, movesNotation);
    }
    else {
        possibleMoves = {};
    }
        
}
//to be connected to the new methods

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
            int pieceId = getPiece(position{ x, y }, chessboard);
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

void sendMove(position from, position to) {
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
//to be chagned: sf::Vector2i --> position

void play(tgui::EditBox::Ptr username, int mode)
{
    tgui::String u_name = username->getText();
    if (u_name != "") {
        gui.removeAllWidgets();
        std::cout << "logged as: " << u_name;
        lastMove.from = position{ -1, -1 };
        gameMode = mode;
        chessboard = initialChessboard;
        currentTurn = true;
        playerColor = false;
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
        label->setPosition({ "28%", "2%" });
        label->setTextSize(24);
        label->setRenderer(GUITheme.getRenderer("Label"));
        gui.add(label);
    }
    

    auto editBoxUsername = tgui::EditBox::create();
    editBoxUsername->setSize({ "66.67%", "12.5%" });
    editBoxUsername->setPosition({ "16.67%", "20%" });
    editBoxUsername->setDefaultText("Nickname");
    editBoxUsername->setMaximumCharacters(80);
    editBoxUsername->setRenderer(GUITheme.getRenderer("EditBox"));
    gui.add(editBoxUsername);

    auto playOffline = tgui::Button::create("Play offline");
    playOffline->setSize({ "50%", "16.67%" });
    playOffline->setPosition({ "25%", "40%" });
    playOffline->setRenderer(GUITheme.getRenderer("Button"));
    gui.add(playOffline);

    auto playOnline = tgui::Button::create("Play online");
    playOnline->setSize({ "50%", "16.67%" });
    playOnline->setPosition({ "25%", "60%" });
    playOnline->setRenderer(GUITheme.getRenderer("Button"));
    gui.add(playOnline);

    auto playBot = tgui::Button::create("Play with bot");
    playBot->setSize({ "50%", "16.67%" });
    playBot->setPosition({ "25%", "80%" });
    playBot->setRenderer(GUITheme.getRenderer("Button"));
    gui.add(playBot);

    // Call the login function when the button is pressed and pass the edit boxes that we created as parameters
    // The "&" in front of "login" can be removed on newer compilers, but is kept here for compatibility with GCC < 8.
    playOffline->onPress(&play, editBoxUsername, 1);
    playOnline->onPress(&play, editBoxUsername, 2);
    playBot->onPress(&play, editBoxUsername, 3);
}

bool processStep(position from, position to) {
    for (const auto pMove : possibleMoves) {
        if (to.x == pMove.to.x and to.y == pMove.to.y) {
            globalMove g;
            g.from.x = from.x;
            g.from.y = from.y;
            g.to.x = to.x;
            g.to.y = to.y;
            g.moveType = pMove.moveType;
           
            
            movePiece(g, chessboard);
            movesNotation += " | " + moveToString(g);
            currentTurn = !currentTurn;
            possibleMoves = {};

            lastMove = g;

            if (gameMode == 1)
                playerColor = currentTurn;

            attackMap = getAttackMap(chessboard, currentTurn, movesNotation);
            position kingPos = getKingPos(currentTurn, chessboard);

            if (checkForCheck(kingPos, attackMap)) {
                check = true;
            }
            else {
                check = false;
            }

            if (gameMode == 2 and currentTurn != playerColor)//move allready has been done so current turn has been changed
                sendMove(from, to);

            if (getAllMoves(chessboard, attackMap, currentTurn, movesNotation).size() == 0) {
                if (check) {
                    std::cout << "Checkmate!\n" << (currentTurn ? "Black" : "White") << " won.\n";
                    gameMode = 0;
                    std::string Winner = currentTurn ? "Black" : "White";

                    if (gameMode == 3 and client != nullptr) {
                        client->recieve = false;
                        if (receive.joinable()) {
                            receive.join();
                        }

                        delete client;
                        client = nullptr;
                    }

                    loadMenu(gui, "Checkmate!\n" + Winner + " won.");
                }
                else {
                    std::cout << "Stalemate!\n Its draw.";
                    gameMode = 0;
                    if (receive.joinable())
                        receive.join();
                    delete client;
                    client = nullptr;
                    loadMenu(gui, "Stalemate!\n Its draw.");
                }
            }

            return true;
            
        }
    }
    return false;
}
//to be chagned: sf::Vector2i --> position

void botMove() {
    

    globalMove move = Ridrisch.FindMove(chessboard, movesNotation, currentTurn);
    selectPiece(move.from);
    processStep(move.from, move.to);
}

void draw() {
    window.clear(sf::Color(255, 255, 255));

    drawBoard();

    //// Attack map & visualisation
    //std::array<std::array<int, 8>, 8> attackedSquaresMap = getAttackMap(chessboard, !currentTurn, movesNotation);
    //drawAttackMap(attackedSquaresMap);
    //if (currentTurn == false)
    //    botGlobalAttackMap = {};
    if (developerMode)
        playerColor = !playerColor;
    if (check) {
        position kingPos = getKingPos(currentTurn, chessboard);
        sf::Vector2i position = playerColor ? sf::Vector2i(kingPos.x, kingPos.y) : sf::Vector2i( 7 - kingPos.x, 7 - kingPos.y );
        drawSquare(position, sf::Color(191, 114, 114, 150));
    }

    if (selectedPos != sf::Vector2i(-1, -1)) {
        sf::Vector2i position = playerColor ? selectedPos : sf::Vector2i(7 - selectedPos.x, 7 - selectedPos.y);
        drawSquare(position, sf::Color(114, 191, 137, 100));
    }

    if (lastMove.from.x != -1)//weak code /XD what a cringe/
    {
        sf::Vector2i position = playerColor ? sf::Vector2i(lastMove.to.x, lastMove.to.y) : sf::Vector2i(7 - lastMove.to.x, 7 - lastMove.to.y);
        drawSquare(position, sf::Color(255, 215, 0, 200));

        position = playerColor ? sf::Vector2i(lastMove.from.x, lastMove.from.y) : sf::Vector2i(7 - lastMove.from.x, 7 - lastMove.from.y);
        drawSquare(position, sf::Color(255, 215, 0, 100));
    }

    for (possibleMove possibleMove : possibleMoves) {
        sf::Vector2i position = playerColor ? sf::Vector2i(possibleMove.to.x, possibleMove.to.y)
            : sf::Vector2i(7 - possibleMove.to.x, 7 - possibleMove.to.y);
        drawCircle(position, sf::Color(133, 133, 133, 100));
    }

    drawPieces();
    window.display();
    
    if (developerMode)
        playerColor = !playerColor;
}

int main()
{
    srand(time(NULL));
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

                position from{ (client->lastMove / 1000) - 1, (client->lastMove / 100 % 10) - 1 };
                position to{ (client->lastMove / 10 % 10) - 1, (client->lastMove % 10) - 1 };

                selectPiece(from);
                if (processStep(from, to)) {
                    draw();
                }
                if(client != nullptr)
                    client->lastMove = 0;
            }

            if(gameMode == 3 and currentTurn != playerColor)
                botMove();


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
            else if (event.type == sf::Event::MouseButtonReleased and event.mouseButton.button == sf::Mouse::Left) {
                if (gameMode != 0) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2i mousePos;

                    if((playerColor and !developerMode) or (!playerColor and developerMode))
                        mousePos = sf::Vector2i(pixelPos.x / squareSize, pixelPos.y / squareSize);
                    else
                        mousePos = sf::Vector2i(7-pixelPos.x / squareSize, 7-pixelPos.y / squareSize);

                    if (selectedPos != mousePos) {
                        selectedPos = mousePos;

                        if (playerColor == currentTurn) {
                            if (moveFrom.x != -1 and moveFrom.y != -1) {
                                //std::cout << "selected position";
                                if (processStep(position{moveFrom.x, moveFrom.y}, position{ selectedPos.x, selectedPos.y })) {
                                    //std::cout << "selected position 2";
                                    selectedPos = sf::Vector2i(-1, -1);
                                    moveFrom = sf::Vector2i(-1, -1);
                                    draw();
                                    if (gameMode == 3) {
                                        botMove();
                                    }
                                }
                                else {
                                    possibleMoves = {};
                                    if (playerColor ? (getPiece(position{ selectedPos.x, selectedPos.y }, chessboard) > 0) : (getPiece(position{ selectedPos.x, selectedPos.y }, chessboard) < 0))
                                    {
                                        selectPiece(position{ selectedPos.x, selectedPos.y });
                                        moveFrom = selectedPos;
                                    }
                                    else {
                                        moveFrom = sf::Vector2i(-1, -1);
                                        selectedPos = sf::Vector2i(-1, -1);
                                    }
                                }
                            }
                            else if (getPiece(position{ selectedPos.x, selectedPos.y }, chessboard) != 0) {
                                selectPiece(position{ selectedPos.x, selectedPos.y });
                                moveFrom = selectedPos;
                            }
                            else
                                selectedPos = sf::Vector2i(-1, -1);
                        }
                            

                        draw();
                    }
                }

                if (gameMode == 0) {


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


