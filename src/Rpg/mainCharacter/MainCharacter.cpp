#include "MainCharacter.h"
#include "LevelSystem.h"
#include <iostream>
#include <math.h>

MainCharacter::MainCharacter(const sf::Vector2f& position, Inventory* inventory, LevelCompleteMenu* levelCompleteMenu)
    : mPosition(position), mSpeed(142.0f), mPriority(0), mLevelSystem(inventory, levelCompleteMenu) {
    mCollisionZone.setSize(sf::Vector2f(24.f, 16.f));
    mCollisionZone.setPosition(sf::Vector2f(mPosition.x + 21.f, mPosition.y + 47.f));
    mCollisionZone.setFillColor(sf::Color::Red);

    mInteractZone.setSize(sf::Vector2f(30.f, 61.f));
    mInteractZone.setPosition(sf::Vector2f(mPosition.x + 18.f, mPosition.y + 1.f));
    mInteractZone.setFillColor(sf::Color::Blue);

    mSprite.setTextureRect({ 0, 0, 64, 64 });
    mAnimations[int(AnimationIndex::IdleUp)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleUp.png", 4, 2.5f);
    mAnimations[int(AnimationIndex::IdleDown)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleDown.png", 6, 1.8f);
    mAnimations[int(AnimationIndex::IdleLeft)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleLeft.png", 6, 2.f);
    mAnimations[int(AnimationIndex::IdleRight)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleRight.png", 6, 2.f);
    mAnimations[int(AnimationIndex::WalkingUp)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkUp.png", 6, 1.6f);
    mAnimations[int(AnimationIndex::WalkingDown)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkDown.png", 6, 1.6f);
    mAnimations[int(AnimationIndex::WalkingLeft)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkLeft.png", 6, 1.52f);
    mAnimations[int(AnimationIndex::WalkingRight)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkRight.png", 6, 1.52f);
}

void MainCharacter::update(float dt, bool inDialogue) {
    sf::Vector2f direction(0.f, 0.f);
    if (!inDialogue) {
        static std::vector<sf::Keyboard::Key> keyOrder;

        auto addKey = [&](sf::Keyboard::Key key) {
            if (std::find(keyOrder.begin(), keyOrder.end(), key) == keyOrder.end()) {
                keyOrder.push_back(key);
            }
        };

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { direction.x -= 1.165f; addKey(sf::Keyboard::A); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { direction.x += 1.165f; addKey(sf::Keyboard::D); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { direction.y -= 1.165f; addKey(sf::Keyboard::W); }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { direction.y += 1.165f; addKey(sf::Keyboard::S); }

        auto removeKey = [&](sf::Keyboard::Key key) {
            keyOrder.erase(std::remove(keyOrder.begin(), keyOrder.end(), key), keyOrder.end());
            };

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { removeKey(sf::Keyboard::A); }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { removeKey(sf::Keyboard::D); }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { removeKey(sf::Keyboard::W); }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { removeKey(sf::Keyboard::S); }

        if (!keyOrder.empty())
            mLastDirection = keyOrder.back();

        if (direction.x != 0.f || direction.y != 0.f) {
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= length;
        }

        if (direction.x == 0.f && direction.y == 0.f) {
            switch (mLastDirection) {
                case sf::Keyboard::A: mCurrentAnimation = AnimationIndex::IdleLeft; break;
                case sf::Keyboard::D: mCurrentAnimation = AnimationIndex::IdleRight; break;
                case sf::Keyboard::W: mCurrentAnimation = AnimationIndex::IdleUp; break;
                case sf::Keyboard::S: mCurrentAnimation = AnimationIndex::IdleDown; break;
                default: break;
            }
        } else {
                switch (mLastDirection) {
                case sf::Keyboard::A: mCurrentAnimation = AnimationIndex::WalkingLeft; break;
                case sf::Keyboard::D: mCurrentAnimation = AnimationIndex::WalkingRight; break;
                case sf::Keyboard::W: mCurrentAnimation = AnimationIndex::WalkingUp; break;
                case sf::Keyboard::S: mCurrentAnimation = AnimationIndex::WalkingDown; break;
                default: break;
            }
        }
    } else {
        switch (mLastDirection) {
            case sf::Keyboard::A: mCurrentAnimation = AnimationIndex::IdleLeft; break;
            case sf::Keyboard::D: mCurrentAnimation = AnimationIndex::IdleRight; break;
            case sf::Keyboard::W: mCurrentAnimation = AnimationIndex::IdleUp; break;
            case sf::Keyboard::S: mCurrentAnimation = AnimationIndex::IdleDown; break;
            default: break;
        }
    }

    mAnimations[int(mCurrentAnimation)].update(dt);
    mAnimations[int(mCurrentAnimation)].applyToSprite(mSprite);

    sf::Vector2f originalPosition = mPosition;
    mPosition += direction * mSpeed * dt;

    mSprite.setPosition(mPosition);
    mCollisionZone.setPosition(sf::Vector2f(mPosition.x + 21.f, mPosition.y + 47.f));
    mInteractZone.setPosition(sf::Vector2f(mPosition.x + 18.f, mPosition.y + 1.f));
}


void MainCharacter::render(sf::RenderWindow& window) {
    //window.draw(mInteractZone);
    //window.draw(mCollisionZone);
    window.draw(mSprite);
}

void MainCharacter::setPosition(const sf::Vector2f& position) {
    mSprite.setPosition(position);
    mCollisionZone.setPosition(sf::Vector2f(mPosition.x + 21.f, mPosition.y + 47.f));
    mInteractZone.setPosition(sf::Vector2f(mPosition.x + 18.f, mPosition.y + 1.f));
    mPosition = position;
}

void MainCharacter::setCenterPosition(const sf::Vector2f& position) {
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    setPosition(sf::Vector2f(position.x - bounds.width / 2.f, position.y - bounds.height / 2.f));
}

sf::Vector2f MainCharacter::getPosition() const {
    return mSprite.getPosition();
}

sf::Vector2f MainCharacter::getCenterPosition() const {
    return sf::Vector2f(mSprite.getPosition().x + mSprite.getLocalBounds().width / 2.f,
                        mSprite.getPosition().y + mSprite.getLocalBounds().height / 2.f);
}

float MainCharacter::getHeight() const {
    return mSprite.getGlobalBounds().height;
}

sf::FloatRect MainCharacter::getBounds() const {
    return mCollisionZone.getGlobalBounds();
}

sf::FloatRect MainCharacter::getInteractBounds() const {
    return mInteractZone.getGlobalBounds();
}

void MainCharacter::setAnimation(int animation) {
    if (animation == 4) {
        mLastDirection = sf::Keyboard::S;
        mCurrentAnimation = AnimationIndex::IdleDown;
    } else if (animation == 3) {
        mLastDirection = sf::Keyboard::W;
        mCurrentAnimation = AnimationIndex::IdleUp;
    } else if (animation == 2) {
        mLastDirection = sf::Keyboard::D;
        mCurrentAnimation = AnimationIndex::IdleRight;
    } else if (animation == 1) {
        mLastDirection = sf::Keyboard::A;
        mCurrentAnimation = AnimationIndex::IdleLeft;
    }

    mAnimations[int(mCurrentAnimation)].applyToSprite(mSprite);
}

int MainCharacter::getAnimation() {
    if (mCurrentAnimation == AnimationIndex::WalkingDown || mCurrentAnimation == AnimationIndex::IdleDown)
        return 4;
    if (mCurrentAnimation == AnimationIndex::WalkingUp || mCurrentAnimation == AnimationIndex::IdleUp)
        return 3;
    if (mCurrentAnimation == AnimationIndex::WalkingRight || mCurrentAnimation == AnimationIndex::IdleRight)
        return 2;
    if (mCurrentAnimation == AnimationIndex::WalkingLeft || mCurrentAnimation == AnimationIndex::IdleLeft)
        return 1;
    return 0;
}

LevelSystem& MainCharacter::getLevelSystem() {
    return mLevelSystem;
}

int MainCharacter::getLevel() const {
    return mLevelSystem.getLevel();
}

int MainCharacter::getXp() const {
    return mLevelSystem.getCurrentXp();
}

int MainCharacter::getXpForNextLevel() const {
    return mLevelSystem.getXpForNextLevel();
}

