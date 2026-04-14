#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(const sf::Vector2f& windowSize, const sf::Vector2f& menuSizeRatio = sf::Vector2f(0.5f, 0.5f));
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
};
