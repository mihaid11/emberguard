#include "LevelCompleteMenu.h"
#include "../gamengine/GameEngine.h"
#include <iostream>

LevelCompleteMenu::LevelCompleteMenu(sf::RenderWindow& window, GameEngine* game, int level)
	: mGame(game), mLevel(level),
	quitButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Quit"),
	playAgainButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Play Again")

{
	if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
		std::cout << "Couldn't load font from file" << std::endl;

	mCompleteText.setFont(mFont);
	mCompleteText.setCharacterSize(50);
	mCompleteText.setString("Thank you for playing the game!");
	mCompleteText.setFillColor(sf::Color::White);
	mCompleteText.setPosition(sf::Vector2f((window.getSize().x - mCompleteText.getGlobalBounds().width) / 2.0f,
		200.0f));

	mMenuShape.setSize(sf::Vector2f(window.getSize().x / 1.f, window.getSize().y / 1.f));
	mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
	mMenuShape.setPosition(sf::Vector2f(0, 0));

	quitButton.setPosition(sf::Vector2f(mCompleteText.getPosition().x + 333.f, mCompleteText.getPosition().y + 177.f));
	playAgainButton.setPosition(sf::Vector2f(mCompleteText.getPosition().x + 333.f, mCompleteText.getPosition().y + 125.f));

	quitButton.setCallback([&]() {
		window.close();
		});

	playAgainButton.setCallback([this]() {
		if (mGame) {
			mGame->init(1);
		}
		else {
			std::cerr << "Error: GameManager is nullptr in returnButton callback." << std::endl;
		}
		});

	mButtons.push_back(playAgainButton);
	mButtons.push_back(quitButton);
}

void LevelCompleteMenu::render(sf::RenderWindow& window)
{
	window.draw(mMenuShape);

	for (auto& button : mButtons) {
		button.render(window);
	}
	window.draw(mCompleteText);
}

void LevelCompleteMenu::handleMouseClick(const sf::Vector2f& mousePos)
{
	for (auto& button : mButtons) {
		if (button.isMouseOver(mousePos)) {
			button.onClick();
		}
	}
}

void LevelCompleteMenu::updateHover(const sf::Vector2f& mousePos)
{
	for (auto& button : mButtons) {
		button.updateHover(mousePos);
	}
}
