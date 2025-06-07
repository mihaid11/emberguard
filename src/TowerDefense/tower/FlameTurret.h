#pragma once
#include "Tower.h"
#include <SFML/Graphics.hpp>
#include "../enemy/Enemy.h"
#include "../projectiles/FlameProjectile.h"

class FlameTurret : public Tower {
public:
    FlameTurret(const sf::Vector2f& position, std::vector<Projectile>& projectiles);

    void update(float dt, std::vector<Enemy>& enemies) override;

private:
    void fireFlame();
};

