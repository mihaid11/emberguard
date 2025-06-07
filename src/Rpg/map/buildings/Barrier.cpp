#include "Barrier.h"

Barrier::Barrier(const sf::Vector2f& position, const sf::Vector2f& size)
    : Entity(position, "", position, size, 0, { 0, 0 }, { 0, 0 }) {
    mShape.setFillColor(sf::Color::Red);
    mShape.setPosition(position);
    mShape.setSize(size);
}

void Barrier::render(sf::RenderWindow& window) {
    //window.draw(mShape);
}

