#pragma once
#include <SFML/Graphics.hpp>

class DrawableEntity {
public:
    virtual ~DrawableEntity() = default;

    virtual void render(sf::RenderWindow& window) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual float getHeight() const = 0;
};
