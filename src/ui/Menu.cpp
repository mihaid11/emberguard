#include "Menu.h"
#include <iostream>

Menu::Menu(const sf::Vector2f& windowSize, const sf::Vector2f& menuSizeRatio) {
    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in base Menu" << std::endl;

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

Menu::Menu(const sf::Vector2f& menuSize, const sf::Vector2f& menuPosition, bool isSubMenu) {
    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in base Menu" << std::endl;

    mBackground.setSize(sf::Vector2f(0.f, 0.f));

    mMenuShape.setSize(menuSize);
    mMenuShape.setPosition(menuPosition);
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
}

void Menu::initializeTitle(const std::string& text, int size) {
    mTitle.setFont(mFont);
    mTitle.setCharacterSize(size);
    mTitle.setFillColor(sf::Color::White);
    mTitle.setString(text);

    mTitle.setOrigin(mTitle.getLocalBounds().left + mTitle.getLocalBounds().width / 2.f,
                     mTitle.getLocalBounds().top + mTitle.getLocalBounds().height / 2.f);
    mTitle.setPosition(sf::Vector2f(mHoveredZoneShape.getPosition().x + mHoveredZoneShape.getSize().x / 2.f,
                                    mHoveredZoneShape.getPosition().y + mHoveredZoneShape.getSize().y / 2.f));

}

void Menu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mBackground);
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);

    if (!mTitle.getString().isEmpty())
        window.draw(mTitle);
}

sf::Text Menu::createMessageText(const sf::Font& font, const std::string& string, const sf::Vector2f& position, int size) {
    sf::Text output;
    output.setFont(font);
    output.setCharacterSize(size);
    output.setFillColor(sf::Color::White);
    output.setString(string);
    output.setPosition(position);
    return output;
}
