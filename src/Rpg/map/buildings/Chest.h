#pragma once
#include "../entities/Entity.h"
#include <SFML/Graphics.hpp>

class Chest : public Entity {
public:
    Chest(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
          const sf::Vector2f& collPosition, const sf::Vector2f& collSize,
          const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
          bool& showMenu);

    void interact() override;

private:
    bool& mShowMenu;
};

