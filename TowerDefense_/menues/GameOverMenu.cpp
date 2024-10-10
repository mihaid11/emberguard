#include "GameOverMenu.h"
#include "../gamengine/GameEngine.h"
#include <iostream>

GameOverMenu::GameOverMenu(sf::RenderWindow& window, GameEngine* game, int level)
	: mGame(game), mLevel(level),
	restartButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Restart"),
	quitButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Quit")
{
	mMenuShape.setSize(sf::Vector2f(270, 140));
	mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
	mMenuShape.setPosition(
		(window.getSize().x - mMenuShape.getSize().x) / 2,
		(window.getSize().y - mMenuShape.getSize().y) / 2
	);

	quitButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 45, mMenuShape.getPosition().y + 77));
	restartButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 45, mMenuShape.getPosition().y + 25));

	restartButton.setCallback([this]() {
		if (mGame) {
			mGame->init(mLevel);
		}
		else {
			std::cerr << "Error: Game is nullptr in restartButton callback." << std::endl;
		}
		});

	quitButton.setCallback([&]() {
		window.close();
		});

	mButtons.push_back(restartButton);
	mButtons.push_back(quitButton);
}

void GameOverMenu::render(sf::RenderWindow& window)
{
	window.draw(mMenuShape);

	for (auto& button : mButtons) {
		button.render(window);
	}
}

void GameOverMenu::handleMouseClick(const sf::Vector2f& mousePos)
{
	for (auto& button : mButtons) {
		if (button.isMouseOver(mousePos)) {
			button.onClick();
		}
	}
}

void GameOverMenu::updateHover(const sf::Vector2f& mousePos)
{
	for (auto& button : mButtons) {
		button.updateHover(mousePos);
	}
}
