#pragma once
#include "../entities/Entity.h"
#include <SFML/Graphics.hpp>
#include <functional>

class Chest : public Entity {
public:
    Chest(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
          const sf::Vector2f& collPosition, const sf::Vector2f& collSize,
          const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
          std::function<void(bool)> openCallback);

    void interact() override;

private:
    std::function<void(bool)> mOpenCallback;
};
