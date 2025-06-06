#include "TowerBlueprintEpic.h"

TowerBlueprintEpic::TowerBlueprintEpic()
	: Item("TowerBlueprint Epic", "Can be used to unlock epic towers!", 4, 150, sf::RectangleShape(sf::Vector2f(50.f, 50.f)))
{
	sf::RectangleShape icon(sf::Vector2f(50.f, 50.f));
	icon.setFillColor(sf::Color(160, 160, 120, 180));
	const_cast<sf::RectangleShape&>(getIcon()) = icon;
}

std::string TowerBlueprintEpic::getType() const
{
	return "TowerBlueprintEpic";
}

