#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>


class Menu {
public:
	sf::RenderWindow* gameWindow;
	sf::RectangleShape bg;
	sf::RectangleShape buttonSolo;
	sf::RectangleShape buttonMultiplayer;
	sf::Vector2f resolution;
	sf::Font titleFont;
	sf::Text title;
	sf::Text soloText;
	sf::Text multipText;
	sf::Color light;
	sf::Color dark;

	Menu(sf::Vector2f res, sf::Font font, sf::RenderWindow* win) {
		gameWindow = win;
		resolution = res;
		titleFont = font;
		bg.setSize(resolution);
		light = sf::Color(173, 239, 209);
		dark = sf::Color(0, 32, 63);
		bg.setFillColor(sf::Color(0, 32, 63));
		bg.setOutlineThickness(-10);
		bg.setOutlineColor(sf::Color(173, 239, 209));
		bg.setPosition(0, 0);
		title.setFont(titleFont);
		title.setString("Snake.IO");
		title.setFillColor(sf::Color(173, 239, 209));
		title.setStyle(sf::Text::Bold);
		title.setPosition(resolution.x / 2 - 225, 125);
		title.setCharacterSize(150);

		buttonSolo = sf::RectangleShape(sf::Vector2f(225, 75));
		buttonSolo.setPosition(resolution.x / 2 - buttonSolo.getSize().x / 2, 350);
		buttonSolo.setFillColor(sf::Color(173, 239, 209));
		soloText = sf::Text("Solo", titleFont, 75);
		soloText.setFillColor(dark);
		soloText.setPosition(buttonSolo.getPosition().x + 55, buttonSolo.getPosition().y -13 );
		buttonMultiplayer = sf::RectangleShape(sf::Vector2f(350, 80));
		buttonMultiplayer.setPosition(resolution.x / 2 - buttonMultiplayer.getSize().x / 2, 455);
		buttonMultiplayer.setFillColor(dark);
		multipText = sf::Text("Multiplayer", titleFont, 75);
		multipText.setFillColor(light);
		multipText.setPosition(buttonMultiplayer.getPosition().x + 20, buttonMultiplayer.getPosition().y - 18);
	}


	void update() {
		isHovering(buttonSolo, soloText);
		isHovering(buttonMultiplayer, multipText);
		gameWindow->draw(bg);
		gameWindow->draw(title);
		gameWindow->draw(buttonSolo);
		gameWindow->draw(soloText);
		gameWindow->draw(buttonMultiplayer);
		gameWindow->draw(multipText);
		

	}
	void isHovering(sf::RectangleShape& box, sf::Text& txt) {
		float mousePosX = sf::Mouse::getPosition(*gameWindow).x;
		float mousePosY = sf::Mouse::getPosition(*gameWindow).y;
		if (mousePosX <= box.getPosition().x + box.getSize().x && mousePosX >= box.getPosition().x) {
			if (mousePosY <= box.getPosition().y + box.getSize().y && mousePosY >= box.getPosition().y)
			{
				box.setFillColor(light);
				txt.setFillColor(dark);
				
			}
			else {
				box.setFillColor(dark);
				txt.setFillColor(light);
			}

		}
	}

	std::string hoveringOn() {
		float mousePosX = sf::Mouse::getPosition(*gameWindow).x;
		float mousePosY = sf::Mouse::getPosition(*gameWindow).y;
		if (mousePosX <= buttonSolo.getPosition().x + buttonSolo.getSize().x 
				&& mousePosX >= buttonSolo.getPosition().x) {
			if (mousePosY <= buttonSolo.getPosition().y + buttonSolo.getSize().y 
				&& mousePosY >= buttonSolo.getPosition().y)
			{
				return "solo";

			}
			else if (mousePosX <= buttonMultiplayer.getPosition().x + buttonMultiplayer.getSize().x
						&& mousePosX >= buttonMultiplayer.getPosition().x) 
				if (mousePosY <= buttonMultiplayer.getPosition().y + buttonMultiplayer.getSize().y
					&& mousePosY >= buttonMultiplayer.getPosition().y) {
					return "mp";
				}
		}
	}
};

class PauseMenu {
public:
	sf::RectangleShape screen;
	sf::Color light;
	sf::Color dark;
	sf::Font mainfont;
	sf::Text txt;
	sf::RenderWindow* window;

	PauseMenu(sf::Vector2f res, sf::Font font, sf::RenderWindow* win) {
		window = win;
		this->mainfont = font;
		screen = sf::RectangleShape(res);
		screen.setFillColor(sf::Color(173, 239, 209));
		screen.setOutlineColor(sf::Color(0, 32, 63));
		screen.setOutlineThickness(-25);
		txt = sf::Text("Paused", mainfont, 125);
		txt.setStyle(sf::Text::Bold);
		txt.setFillColor(sf::Color(0, 32, 63));
		txt.setPosition(res.x / 2 - 165, 225);
	}

	void update() {
		window->draw(screen);
		window->draw(txt);
	}
};















#endif MENU_h