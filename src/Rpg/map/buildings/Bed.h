#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "../../../TimeSystem.h"

class GameManager;

class Bed :	public Entity
{
public:
	Bed(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
		const sf::Vector2f& collPosition, const sf::Vector2f& collSize, 
		const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize, TimeSystem& timeSystem,
		GameManager* gameManager);

	void interact() override;

private:
	TimeSystem& mTimeSystem;
	GameManager* mGameManager;
};

