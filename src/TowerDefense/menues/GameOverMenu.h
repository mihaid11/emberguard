#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include <vector>

class GameEngine;
class GameManager;

class GameOverMenu
{
public:
	GameOverMenu(sf::RenderWindow& window, GameEngine* game, GameManager* gameManager,
		int level, int crystals, std::vector<int>& availableTowers);

	void render(sf::RenderWindow& window);
	void handleMouseClick(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);

private:
	sf::RectangleShape mMenuShape;
	std::vector<Button> mButtons;
	Button restartButton;
	Button exitButton;

	GameEngine* mGame;
	GameManager* mGameManager;
	int mLevel;
	int mCrystals;
	std::vector<int>& mAvailableTowers;
};

