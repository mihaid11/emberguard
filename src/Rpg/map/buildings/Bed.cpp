#include "Bed.h"
#include <iostream>
#include "../../../GameManager.h"

Bed::Bed(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
	const sf::Vector2f& collPosition, const sf::Vector2f& collSize,
	const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize, TimeSystem& timeSystem,
	GameManager* gameManager)
	: Entity(position, filename, collPosition, collSize, 1, interactPosition, interactSize), mTimeSystem(timeSystem),
	mGameManager(gameManager)
{
	mSprite.setScale(sf::Vector2f(sizeIncrement, sizeIncrement));
}

void Bed::interact()
{
	std::cout << "Good night!" << std::endl;
	mGameManager->startTransition(true);
}
