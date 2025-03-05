#include "GameOverMenu.h"
#include "../gamengine/GameEngine.h"
#include <iostream>

GameOverMenu::GameOverMenu(sf::RenderWindow& window, GameEngine* game, int level)
	: mGame(game), mLevel(level),
	restartButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Restart"),
	quitButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Quit")
{
	if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
		std::cout << "Couldn't load font from file" << std::endl;

	mGameOverText.setFont(mFont);
	mGameOverText.setCharacterSize(50);
	mGameOverText.setString("Game Over!");
	mGameOverText.setFillColor(sf::Color::White);
	mGameOverText.setPosition(sf::Vector2f((window.getSize().x - mGameOverText.getGlobalBounds().width) / 2.0f,
		200.0f));

	mMenuShape.setSize(sf::Vector2f(window.getSize().x / 1.f, window.getSize().y / 1.f));
	mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
	mMenuShape.setPosition(sf::Vector2f(0.f, 0.f));

	quitButton.setPosition(sf::Vector2f(mGameOverText.getPosition().x + 62.f, mGameOverText.getPosition().y + 177.f));
	restartButton.setPosition(sf::Vector2f(mGameOverText.getPosition().x + 62.f, mGameOverText.getPosition().y + 125.f));

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

	window.draw(mGameOverText);
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
