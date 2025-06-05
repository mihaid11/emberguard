#include "TowerBlueprintRare.h"

TowerBlueprintRare::TowerBlueprintRare()
	: Item("TowerBlueprint Rare", "Can be used to unlock rare towers!", 3, 100, sf::RectangleShape(sf::Vector2f(50.f, 50.f)))
{
	sf::RectangleShape icon(sf::Vector2f(50.f, 50.f));
	icon.setFillColor(sf::Color(120, 120, 20, 80));
	const_cast<sf::RectangleShape&>(getIcon()) = icon;
}

std::string TowerBlueprintRare::getType() const
{
	return "TowerBlueprintRare";
}

