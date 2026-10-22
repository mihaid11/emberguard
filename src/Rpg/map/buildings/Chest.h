#pragma once
#include "../entities/Entity.h"
#include <SFML/Graphics.hpp>
#include <functional>

class Chest : public Entity {
public:
    Chest(const sf::Vector2f& position, const std::string filename,
          const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
          std::function<void(bool)> openCallback, const sf::Vector2f& collPosition = {0.f, 0.f},
          const sf::Vector2f& collSize = {0.f, 0.f});

    void interact() override;

private:
    std::function<void(bool)> mOpenCallback;
};
