#pragma once
#include <SFML/Graphics.hpp>
#include "../../entities/DrawableEntity.h"

class Entity : public DrawableEntity
{
public:
	Entity(const sf::Vector2f& position, const std::string filename,
		const sf::Vector2f& collPosition, const sf::Vector2f& collSize, int canInteract,
		const sf::Vector2f& interactPos, const sf::Vector2f& interactSize);
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
	sf::Texture *mTexture;
	sf::RectangleShape mCollisionZone;
	sf::RectangleShape mInteractableZone;
	int mCanInteract;
};

