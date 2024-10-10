#pragma once
#include <SFML/Graphics.hpp>
#include "../enemy/Enemy.h"
#include <vector>
#include "../projectiles/Projectile.h"

class Player
{
public:
	Player(const sf::Vector2f& position, std::vector<Projectile>& projectiles);

	void update(float dt, const std::vector<Enemy>& enemies);
	void render(sf::RenderWindow& window);
	sf::Vector2f getPosition();
	bool isInRange(const Enemy& enemy) const;

	float getAttackRange() const;
	float getPlacementRange() const;
	sf::Vector2f getPosition() const;
	float getDamage() const;
	float getFireRate() const;

	void takeDamage(float amount);
	float getHealth() const;
	void setHealth(float health);

	sf::FloatRect getBounds() const;
	void setPosition(const sf::Vector2f& position);

private:
	sf::CircleShape mShape;
	sf::Vector2f mPosition;
	Enemy* mTarget;
	float mSpeed;
	float mDamage;
	float mFireRate;
	float mTimeSinceLastShot;
	float mRange;
	std::vector<Projectile>& mProjectiles;
	bool mIsMoving;
	float mHealth;
	static constexpr float PlacementRange = 180.0f;

	void fireProjectile();
};

