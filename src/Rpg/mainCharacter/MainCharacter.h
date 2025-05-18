#pragma once
#include <SFML/Graphics.hpp>
#include "../map/GameMap.h"
#include "../../Animation.h"
#include "../entities/DrawableEntity.h"

class MainCharacter : public DrawableEntity
{
public:
	MainCharacter(const sf::Vector2f& position, GameMap& map);

	void update(float dt, bool inDialogue);
	void render(sf::RenderWindow& window) override;

	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const override;
	float getHeight() const override;
	sf::FloatRect getBounds() const;
	void setAnimation(int animation);
	int getAnimation();

private:
	enum class AnimationIndex {
		IdleUp,
		IdleDown,
		IdleLeft,
		IdleRight,
		WalkingUp,
		WalkingDown,
		WalkingLeft,
		WalkingRight,
		Count
	};
	sf::Vector2f mPosition;
	float mSpeed;
	GameMap& mMap;
	int mPriority;

	sf::Sprite mSprite;
	Animation mAnimations[int(AnimationIndex::Count)];
	AnimationIndex mCurrentAnimation = AnimationIndex::IdleDown;
	sf::RectangleShape mCollisionZone;
	sf::Keyboard::Key mLastDirection = sf::Keyboard::A;
};

