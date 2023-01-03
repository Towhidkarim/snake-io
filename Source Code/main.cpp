#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entities.h"
#include "Menu.h"

enum gameState {inGame = 0, menu, pause};

int main()
{
    //              Constants, declairations and other function, method calls           //
   //----------------------------------------------------------------------------------//

    const sf::Vector2f resolution = sf::Vector2f(1280, 720);
    const std::string windowName = "Snake.IO";
    const int frameRate = 60;
    int r = 0;
    srand(time(0));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::Texture bgtex;
    sf::Font mainFont;
    sf::Text score;
    gameState currentGameState = menu;
    bool multiPlayer = true;

    if (!mainFont.loadFromFile("font.ttf")) std::cout << "error";
    score.setFont(mainFont);
    score.setString("Score: 0");
    score.setCharacterSize(50);
    score.setFillColor(sf::Color(10, 113, 173));
    score.setStyle(sf::Text::Bold);

    if (!bgtex.loadFromFile("grid.png")) {
        std::cout << "error";
    }
    
    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), windowName, sf::Style::Default, settings);
    window.setFramerateLimit(frameRate);

    sf::Clock masterClock;
    float delta = (float)1000 / 60;
    sf::Sprite back;

       //                           Game Background                         //

    bgtex.setRepeated(true);
    back.setTexture(bgtex);
    back.setColor(sf::Color(255, 227, 238));
    back.setTextureRect(sf::IntRect(0, 0, 5000, 5000));
    back.setPosition(-2500, -2500);

    //                              Player intialization                   //

    std::vector<SnakeHead> playerList;
    playerList.push_back(SnakeHead(20, 150, 0, &window, &delta));
    //playerList.push_back(SnakeHead(20, -150, 0, &window, &delta));
    //SnakeHead player2(20, 2600, 2500, &window, &delta);


    //                      List of views for all cases

    sf::View playerView(sf::Vector2f(playerList[0].pos), sf::Vector2f(1280, 720));
    sf::View views[2];
    views[0] = sf::View(sf::Vector2f(playerList[0].pos), sf::Vector2f(640, 720));
    views[1] = sf::View(sf::Vector2f(playerList[1].pos), sf::Vector2f(640, 720));
    views[0].setViewport(sf::FloatRect(0.f, 0.f, 0.4985f, 1.f));
    views[1].setViewport(sf::FloatRect(0.5015f, 0.f, 0.5f, 1.f));

    //                                  Menu
    Menu mainMenu(resolution, mainFont, &window);
    PauseMenu pauseMenu(resolution, mainFont, &window);


    //                                  Fooood

    std::vector<Food> foodlist;
    for (int i = 0; i <= 500; i++) {
        int randx = (rand() % 5000) - 2500;
        int randy = (rand() % 5000) - 2500;
        foodlist.push_back(Food(10, randx, randy, &window, &delta, &playerList[0].object));
        
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape && currentGameState != pause) currentGameState = pause;
                else if (event.key.code == sf::Keyboard::Escape && currentGameState == pause) currentGameState = inGame;
            }
        }

        delta = masterClock.restart().asSeconds();
        window.clear();

        switch (currentGameState)
        {
        case inGame:
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape && currentGameState != pause) currentGameState = pause;
            }
            for (int p = 0; p < playerList.size(); p++) {
                if (multiPlayer) {
                    views[p].setCenter(playerList[p].pos);
                    window.setView(views[p]);
                }
                else 
                {
                    playerView.setCenter(playerList[p].pos);
                    window.setView(playerView);
                }
                window.draw(back);

            for (int i = 0; i < foodlist.size(); i++) {
                float distance = foodlist[i].distSq(playerList[p].pos);
                if (distance < 1000000) {
                    if (distance + pow(playerList[p].radius, 2) < 3200) {
                        foodlist[i].moveToHead(playerList[p].pos);
                    }
                    if (distance > 100)
                        foodlist[i].update();
                    else if (distance <= 100)
                    {
                        playerList[p].updateScore(1);
                        int randx = (rand() % 5000) - 2500;
                        int randy = (rand() % 5000) - 2500;
                        foodlist.push_back(Food(10, randx, randy, &window, &delta, &playerList[0].object));
                        foodlist.erase(foodlist.begin() + i);
                    }

                }
            }
            if(multiPlayer)
            playerList[1].reDrawBody();
            playerList[0].reDrawBody();
            playerList[p].update();
            
            score.setString("Score: " + std::to_string((int)playerList[p].score));
            score.setFillColor(playerList[p].object.getOutlineColor());
            if (multiPlayer)
                score.setPosition(views[p].getCenter().x - 300, views[p].getCenter().y - 350);
            else score.setPosition(playerView.getCenter().x - 500, playerView.getCenter().y - 350);
            window.draw(score);
        }
            break;

        case menu:
            mainMenu.update();
            window.pollEvent(event);
                if (event.type == sf::Event::MouseButtonPressed) {

                   if(mainMenu.hoveringOn() == "mp") {
                        playerList.push_back(SnakeHead(20, -150, 0, &window, &delta));
                        playerList[1].scheme = ARROW;
                        playerList[1].setColor(sf::Color(250, 60, 98), sf::Color(181, 43, 71));
                        currentGameState = inGame;
                    }
                   else if (mainMenu.hoveringOn() == "solo") {
                       multiPlayer = false;
                       currentGameState = inGame;
                   }
                }
            
            break;
        case pause:
            playerView.setCenter(resolution.x / 2, resolution.y / 2);
            window.setView(playerView);
            pauseMenu.update();
            break;
        default:
            break;
        }


        window.display();



    }

    return 0;
}

