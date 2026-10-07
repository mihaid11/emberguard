#pragma once
#include <SFML/Graphics.hpp>
#include "../../../core/ResourceManager.h"
#include "DrawableEntity.h"

class Entity : public DrawableEntity {
public:
    Entity(const sf::Vector2f& position, const sf::Vector2f& size,
           const std::string filename, const sf::Vector2f& collPosition = {0.f, 0.f},
           const sf::Vector2f& collSize = {0.f, 0.f}, int canInteract = 0,
           const sf::Vector2f& interactPos = {0.f, 0.f}, const sf::Vector2f& interactSize = {0.f, 0.f});
    virtual ~Entity() = default;

    virtual void render(sf::RenderWindow& window) override;
    virtual void update();
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const override;
    sf::Vector2f getInteractPosition() const;
    float getHeight() const override;
    int isInteractable() const;
    sf::FloatRect getInteractBounds() const;
    virtual void interact();

protected:
    sf::Sprite mSprite;

    sf::RectangleShape mCollisionZone;
    sf::RectangleShape mInteractableZone;
    int mCanInteract;
};
