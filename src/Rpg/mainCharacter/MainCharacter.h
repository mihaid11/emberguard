#pragma once
#include <SFML/Graphics.hpp>
#include "../../Animation.h"
#include "../map/entities/DrawableEntity.h"
#include "../inventory/Inventory.h"
#include "LevelSystem.h"
#include "../../TowerDefense/menues/LevelCompleteMenu.h"

class MainCharacter : public DrawableEntity {
public:
    MainCharacter(const sf::Vector2f& position, Inventory* inventory, LevelCompleteMenu* levelCompleteMenu);

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

    LevelSystem& getLevelSystem();
    int getLevel() const;
    void setLevel(int level);

    int getXp() const;
    int getXpForNextLevel() const;
    void setXp(int xp);

    sf::Sprite& getSprite();
    sf::Sprite& getIconSprite();

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

    LevelSystem mLevelSystem;

    sf::Keyboard::Key mLastDirection = sf::Keyboard::A;
    sf::Sprite mSprite;
    Animation mAnimations[int(AnimationIndex::Count)];
    AnimationIndex mCurrentAnimation = AnimationIndex::IdleDown;

    sf::Sprite mIconSprite;
    Animation mIconTexture;

    sf::RectangleShape mCollisionZone;
    sf::RectangleShape mInteractZone;
};

