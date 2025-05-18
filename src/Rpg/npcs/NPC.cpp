#include "NPC.h"
#include <cstdlib>
#include <iostream>
#include <math.h>

NPC::NPC(const sf::Vector2f& position)
    : mInteractionRadius(60.0f), mPaused(false), mSpeed(35.0f),
    mCurrentWaypoint(0), mIsPaused(false), mPauseDuration(3.0f), mPauseTimer(0.0f), mPriority(0),
    mShowInteract(false) {

    mSprite.setTextureRect({ 0, 0, 64, 64 });
    mAnimations[int(AnimationIndex::IdleUp)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleUp.png", 4, 2.5f);
    mAnimations[int(AnimationIndex::IdleDown)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleDown.png", 6, 1.8f);
    mAnimations[int(AnimationIndex::IdleLeft)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleLeft.png", 6, 2.f);
    mAnimations[int(AnimationIndex::IdleRight)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/idleRight.png", 6, 2.f);
    mAnimations[int(AnimationIndex::WalkingUp)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkUp.png", 6, 1.6f);
    mAnimations[int(AnimationIndex::WalkingDown)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkDown.png", 6, 1.6f);
    mAnimations[int(AnimationIndex::WalkingLeft)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkLeft.png", 6, 1.52f);
    mAnimations[int(AnimationIndex::WalkingRight)] = Animation(0, 0, 64, 64, "assets/sprites/mainCharacter/walkRight.png", 6, 1.52f);

    mIconSprite.setTextureRect({ 0, 0, 64, 64 });
    mIconTexture = Animation(0, 0, 28, 64, "assets/sprites/mainCharacter/idleDown.png", 1, 0.0f);
    mIconTexture.applyToSprite(mIconSprite);
    mIconSprite.setScale({ 2.7f, 2.7f });

    mInteractCircle.setRadius(7.5f);
    mInteractCircle.setFillColor(sf::Color(30, 30, 30, 200));
    mInteractCircle.setOutlineColor(sf::Color::White);
    mInteractCircle.setOutlineThickness(0.7f);

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf")) {
        std::cerr << "Failed to load font for NPC!" << std::endl;
    }

    mInteractText.setFont(mFont);
    mInteractText.setCharacterSize(7);
    mInteractText.setFillColor(sf::Color::White);
    mInteractText.setString("E");
}

void NPC::setDialogue(const std::string& key, const Dialogue& dialogue)
{
    mDialogues[key] = dialogue;
}

void NPC::setActiveDialogue(const std::string& key) {
    auto it = mDialogues.find(key);
    if (it != mDialogues.end()) {
        mActiveDialogueKey = key;
        mDialogueManager.startDialogue(it->second);
    }
    else {
        std::cerr << "Dialogue key not found: " << key << std::endl;
    }
}

Dialogue NPC::getActiveDialogue()
{
    return mDialogues[mActiveDialogueKey];
}

std::string NPC::getCurrentDialogue()
{
    return mDialogueManager.getCurrentDialogueText();
}

bool NPC::hasMoreDialogue() const
{
    return mDialogueManager.isDialogueInProgress();
}

void NPC::resetDialogue()
{
    mDialogueManager.resetDialogue();
}

void NPC::render(sf::RenderWindow& window)
{
    window.draw(mSprite);
    if (mShowInteract) {
        window.draw(mInteractCircle);
        window.draw(mInteractText);
    }
}

bool NPC::isPlayerClose(const sf::Vector2f& playerPosition) const {
    float distance = std::sqrt(std::pow(playerPosition.x - mSprite.getPosition().x, 2.f) +
        std::pow(playerPosition.y - mSprite.getPosition().y, 2.f));
    return distance <= mInteractionRadius;
}

void NPC::advanceDialogue()
{
    if (mDialogueManager.isDialogueInProgress()) {
        mDialogueManager.getCurrentDialogue().indexIncrement();
        std::cout << "Dialogue index incremented: " << mDialogueManager.getCurrentDialogue().getCurrentIndex() << std::endl;
    }
}

void NPC::update(float dt) {
    updatePause(dt);

    if (mIsPaused || mPaused) {
        if (mPriority == 4)
            mCurrentAnimation = AnimationIndex::IdleDown;
        else if (mPriority == 2)
            mCurrentAnimation = AnimationIndex::IdleRight;
        else if (mPriority == 1)
            mCurrentAnimation = AnimationIndex::IdleLeft;
        else if (mPriority == 3)
            mCurrentAnimation = AnimationIndex::IdleUp;

        mAnimations[int(mCurrentAnimation)].update(dt);
        mAnimations[int(mCurrentAnimation)].applyToSprite(mSprite);
        return;
    }

    if (mPath.empty()) return;

    sf::Vector2f target = mPath[mCurrentWaypoint];
    sf::Vector2f direction = target - mSprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

      
    if (std::abs(direction.x) > std::abs(direction.y)) {
        if (direction.x > 0.f) {
            mCurrentAnimation = AnimationIndex::WalkingRight;
            mPriority = 2;
        }
        else {
            mCurrentAnimation = AnimationIndex::WalkingLeft;
            mPriority = 1;
        }
    }
    else {
        if (direction.y > 0.f) {
            mCurrentAnimation = AnimationIndex::WalkingDown;
            mPriority = 4;
        }
        else {
            mCurrentAnimation = AnimationIndex::WalkingUp;
            mPriority = 3;
        }
    }

    mAnimations[int(mCurrentAnimation)].update(dt);
    mAnimations[int(mCurrentAnimation)].applyToSprite(mSprite);

    mInteractCircle.setPosition(mSprite.getPosition());
    mInteractText.setPosition({ mInteractCircle.getPosition().x + 5.9f,
                                    mInteractCircle.getPosition().y + 2.8f });

    if (distance < 5.0f) {
        // Move to the next waypoint
        mCurrentWaypoint = (mCurrentWaypoint + 1) % mPath.size();

        if (rand() % 10 < 4)
            startPause(3.0f);
    }
    else {
        // Normalize direction and move
        direction /= distance;
        mSprite.move(direction * mSpeed * dt);
    }
}

void NPC::resumeMovement()
{
    mPaused = false;
}

void NPC::pauseMovement()
{
    mPaused = true;
}

sf::Vector2f NPC::getPosition() const
{
    return mSprite.getPosition();
}

float NPC::getHeight() const
{
    return mSprite.getGlobalBounds().height;
}

sf::FloatRect NPC::getBounds() const
{
    return mSprite.getGlobalBounds();
}

std::string NPC::getActiveDialogueKey() const
{
    return mActiveDialogueKey;
}

void NPC::setPosition(const sf::Vector2f& position)
{
    mSprite.setPosition(position);
}

int NPC::getCurrentWaypoint() const
{
    return (int)mCurrentWaypoint;
}

void NPC::setCurrentWaypoint(int waypoint)
{
    mCurrentWaypoint = waypoint;
}

sf::Color NPC::getColor() const
{
    return  mShape.getFillColor();
}

void NPC::setInteractPosition(const sf::Vector2f& position)
{
    mInteractCircle.setPosition(position);
    mInteractText.setPosition({ mInteractCircle.getPosition().x + 5.9f,
                                    mInteractCircle.getPosition().y + 2.8f });
}

void NPC::setAnimationOpposing(int animationIndex)
{
    if (animationIndex == 1) {
        mPriority = 2;
        mCurrentAnimation = AnimationIndex::IdleRight;
    }
    else if (animationIndex == 2) {
        mPriority = 1;
        mCurrentAnimation = AnimationIndex::IdleLeft;
    }
    else if (animationIndex == 3) {
        mPriority = 4;
        mCurrentAnimation = AnimationIndex::IdleDown;
    }
    else if (animationIndex == 4) {
        mPriority = 3;
        mCurrentAnimation = AnimationIndex::IdleUp;
    }
    mAnimations[int(mCurrentAnimation)].applyToSprite(mSprite);
}

void NPC::setInteract(bool showInteract)
{
    mShowInteract = showInteract;
}

sf::Sprite& NPC::getSprite()
{
    return mIconSprite;
}

void NPC::startPause(float duration) {
    mIsPaused = true;
    mPauseDuration = duration;
    mPauseStartTime = std::chrono::steady_clock::now();
}

void NPC::updatePause(float dt) {
    if (mIsPaused) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - mPauseStartTime;
        if (elapsed.count() >= mPauseDuration) {
            mIsPaused = false;
        }
    }
}

