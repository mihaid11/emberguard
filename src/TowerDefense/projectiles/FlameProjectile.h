#pragma once
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include "../enemy/Enemy.h"

class FlameProjectile : public Projectile
{
public:
	FlameProjectile(const sf::Vector2f& position, Enemy* target, float speed, float damage);

	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	float mBurnDuration;
	bool mGroundBurned;
	sf::ConvexShape mShape;
};

