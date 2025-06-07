#include "Wood.h"

Wood::Wood()
    : Item("Wood", "A piece of wood, useful for crafting.", 1, 5, sf::RectangleShape(sf::Vector2f(50.f, 50.f))) {
    sf::RectangleShape icon(sf::Vector2f(50.f, 50.f));
    icon.setFillColor(sf::Color(139, 69, 19));
    const_cast<sf::RectangleShape&>(getIcon()) = icon;
}

std::string Wood::getType() const {
    return "Wood";
}

