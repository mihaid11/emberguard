#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.h"

class GameEngine;

class SmallMenu
{
public:
	SmallMenu(sf::RenderWindow& window, GameEngine* game, int level);

	void render(sf::RenderWindow& window);
	void handleMouseClick(const sf::Vector2f& mousePos);
	void updateHover(const sf::Vector2f& mousePos);

	bool isVisible() const;
	void show();
	void hide();

private:
	sf::RectangleShape mMenuBackground;

	std::vector<Button> mButtons;
	Button quitButton;
	Button restartButton;

	GameEngine* mGame;
	int mLevel;

	bool mIsVisible;
};

