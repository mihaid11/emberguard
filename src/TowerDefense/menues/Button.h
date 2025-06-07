#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text);

    void render(sf::RenderWindow& window);
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    void updateHover(const sf::Vector2f& mousePos);
    void setCallback(std::function<void()> callback);
    void onClick();

    void setPosition(const sf::Vector2f& position);
    void setText(const std::string& text);

    void setBackgroundColor(sf::Color color);
    int getIfHovered();
    const sf::Vector2f& getPosition() const;
    const sf::Vector2f& getSize() const;

private:
    sf::RectangleShape mButtonShape;
    sf::Text mButtonText;
    std::function<void()> mCallback;
    sf::Font mFont;

    sf::Vector2f mSize;
    int mIsHovered;
    sf::Vector2f mPosition;
};

