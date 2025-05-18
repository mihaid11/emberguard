#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>

class Barrier : public Entity 
{
public:
	Barrier(const sf::Vector2f& position, const sf::Vector2f& size);

	void render(sf::RenderWindow& window) override;
private:
	sf::RectangleShape mShape;
};