#pragma once
#include "Projectile.h"

class LightingChain : public Projectile {
public:
    LightingChain(const sf::Vector2f& position, Enemy* target, float speed, float damage);

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::VertexArray mLaserLine;
};

