#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include <vector>

class GameEngine;

class GameOverMenu
{
public:
	GameOverMenu(sf::RenderWindow& window, GameEngine* game, int level);

	void render(sf::RenderWindow& window);
	void handleMouseClick(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);

private:
	sf::RectangleShape mMenuShape;

	std::vector<Button> mButtons;
	Button restartButton;
	Button quitButton;
	sf::Font mFont;
	sf::Text mGameOverText;

	GameEngine* mGame;
	int mLevel;
};

