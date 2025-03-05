#include "Button.h"
#include <iostream>

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text)
    : mSize(size)
{
    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font" << std::endl;

    mButtonShape.setPosition(position);
    mButtonShape.setSize(size);
    mButtonShape.setFillColor(sf::Color(0, 0, 0, 200));
    mButtonShape.setOutlineThickness(2);
    mButtonShape.setOutlineColor(sf::Color(120, 120, 120, 210));

    mButtonText.setFont(mFont);
    mButtonText.setString(text);
    mButtonText.setCharacterSize(18);
    mButtonText.setFillColor(sf::Color::White);

    // Center the text within the button
    sf::FloatRect textBounds = mButtonText.getLocalBounds();
    mButtonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    mButtonText.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
}

void Button::render(sf::RenderWindow& window) {
    window.draw(mButtonShape);
    window.draw(mButtonText);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return mButtonShape.getGlobalBounds().contains(mousePos);
}

void Button::updateHover(const sf::Vector2f& mousePos) {
    if (isMouseOver(mousePos)) {
        mButtonShape.setFillColor(sf::Color(120, 120, 120, 210));
    }
    else {
        mButtonShape.setFillColor(sf::Color(0, 0, 0, 200));
    }
}

void Button::setCallback(std::function<void()> callback) {
    mCallback = callback;
}

void Button::onClick() {
    if (mCallback) {
        mCallback();
    }
}

void Button::setPosition(const sf::Vector2f& position)
{
    mButtonShape.setPosition(position);
    mButtonText.setPosition(position.x + mSize.x / 2.0f, position.y + mSize.y / 2.0f);
}

void Button::setText(const std::string& text)
{
    mButtonText.setString(text);
}
