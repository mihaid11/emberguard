#include "TowerBlueprint.h"

TowerBlueprint::TowerBlueprint()
	: Item("TowerBlueprint", "Can be used to unlock towers!", 2, 50, sf::RectangleShape(sf::Vector2f(50.f, 50.f)))
{
	sf::RectangleShape icon(sf::Vector2f(50.f, 50.f));
	icon.setFillColor(sf::Color(100, 100, 10, 50));
	const_cast<sf::RectangleShape&>(getIcon()) = icon;
}

std::string TowerBlueprint::getType() const
{
	return "TowerBlueprint";
}
