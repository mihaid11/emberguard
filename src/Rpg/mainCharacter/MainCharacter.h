#pragma once
#include <SFML/Graphics.hpp>
#include "../../Animation.h"
#include "../map/entities/DrawableEntity.h"

class MainCharacter : public DrawableEntity {
public:
    MainCharacter(const sf::Vector2f& position);

    void update(float dt, bool inDialogue);
    void render(sf::RenderWindow& window) override;

    void setPosition(const sf::Vector2f& position);
    void setCenterPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const override;

    sf::Vector2f getCenterPosition() const;
    float getHeight() const override;

    sf::FloatRect getBounds() const;
    sf::FloatRect getInteractBounds() const;

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
    int mPriority;

    sf::Keyboard::Key mLastDirection = sf::Keyboard::A;
    sf::Sprite mSprite;
    Animation mAnimations[int(AnimationIndex::Count)];
    AnimationIndex mCurrentAnimation = AnimationIndex::IdleDown;

    sf::RectangleShape mCollisionZone;
    sf::RectangleShape mInteractZone;
};

