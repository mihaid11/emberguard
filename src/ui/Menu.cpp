#include "Menu.h"

Menu::Menu(const sf::Vector2f& windowSize, const sf::Vector2f& menuSizeRatio) {
    mBackground.setSize(windowSize);
    mBackground.setFillColor(sf::Color(50, 50, 50, 185));
    mBackground.setPosition(sf::Vector2f(0, 0));

    mMenuShape.setSize(sf::Vector2f(windowSize.x * menuSizeRatio.x, windowSize.y * menuSizeRatio.y));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition((windowSize.x - mMenuShape.getSize().x) / 2.0f,
                           (windowSize.y - mMenuShape.getSize().y) / 2.0f);

    mHoveredZoneShape.setSize(sf::Vector2f(mMenuShape.getSize().x, 50.f));
    mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
    mHoveredZoneShape.setPosition(mMenuShape.getPosition());
}

void Menu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mBackground);
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);
}
