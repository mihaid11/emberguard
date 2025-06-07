#include "TowerBlueprintMythic.h"

TowerBlueprintMythic::TowerBlueprintMythic()
    : Item("TowerBlueprint Mythic", "Can be used to unlock mythic towers!", 5, 250, sf::RectangleShape(sf::Vector2f(50.f, 50.f))) {
    sf::RectangleShape icon(sf::Vector2f(50.f, 50.f));
    icon.setFillColor(sf::Color(160, 160, 120, 180));
    const_cast<sf::RectangleShape&>(getIcon()) = icon;
}

std::string TowerBlueprintMythic::getType() const {
    return "TowerBlueprintMythic";
}

