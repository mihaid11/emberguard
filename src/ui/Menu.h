#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(const sf::Vector2f& windowSize, const sf::Vector2f& menuSizeRatio = sf::Vector2f(0.5f, 0.5f));
    Menu(const sf::Vector2f& menuSize, const sf::Vector2f& menuPosition, bool isSubMenu);
    virtual ~Menu() = default;

    virtual void update(float dt) = 0;
    virtual void handleMouseClick(const sf::Vector2f& mousePos) = 0;
    virtual void updateHover(const sf::Vector2f& mousePos) = 0;
    virtual void render(sf::RenderWindow& window);

    void setActive(bool active) { mIsActive = active; }
    bool isActive() const { return mIsActive; }
    void toggle() { mIsActive = !mIsActive; }

protected:
    sf::RectangleShape mBackground;
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mHoveredZoneShape;

    bool mIsActive = false;

    sf::Font mFont;
    sf::Text mTitle;

    void initializeTitle(const std::string& text, int size = 20);
    sf::Text createMessageText(const sf::Font& font, const std::string& string, const sf::Vector2f& position, int size = 16);
};
