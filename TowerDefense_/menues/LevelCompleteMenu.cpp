#include "LevelCompleteMenu.h"
#include "../gamengine/GameEngine.h"
#include <iostream>

LevelCompleteMenu::LevelCompleteMenu(sf::RenderWindow& window, GameEngine* game, int level)
	: mGame(game), mLevel(level),
	quitButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Quit"),
	playAgainButton(sf::Vector2f(0, 0), sf::Vector2f(175, 40), "Play Again")

{
	mMenuShape.setSize(sf::Vector2f(270,140));
	mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
	mMenuShape.setPosition(
		(window.getSize().x - mMenuShape.getSize().x) / 2,
		(window.getSize().y - mMenuShape.getSize().y) / 2
	);

	quitButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 45, mMenuShape.getPosition().y + 77));
	playAgainButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + 45, mMenuShape.getPosition().y + 25));

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

	sf::Text completeText;
	sf::Font font;
	if (!font.loadFromFile("gameFont.ttf")) {
		std::cerr << "Couldn't load font from file" << std::endl;
	}
	completeText.setFont(font);
	completeText.setCharacterSize(50);
	completeText.setString("Thank you for playing the game!");
	completeText.setFillColor(sf::Color::White);
	completeText.setPosition(sf::Vector2f((window.getSize().x - completeText.getGlobalBounds().width) / 2.0f,
		200.0f));
	window.draw(completeText);
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
