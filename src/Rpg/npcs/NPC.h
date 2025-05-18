#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include "../dialogueSystem/DialogueManager.h"
#include "../../Animation.h"
#include "../entities/DrawableEntity.h"

class NPC : public DrawableEntity
{
public:
    NPC(const sf::Vector2f& position);

    void advanceDialogue();
    void update(float dt);

    void setDialogue(const std::string& key, const Dialogue& dialogue);
    void setActiveDialogue(const std::string& key);
    Dialogue getActiveDialogue();

    std::string getCurrentDialogue();
    bool hasMoreDialogue() const;
    void resetDialogue();

    void resumeMovement();
    void pauseMovement();
    sf::Vector2f getPosition() const override;
    float getHeight() const override;
    sf::FloatRect getBounds() const;
    std::string getActiveDialogueKey() const;
    void setPosition(const sf::Vector2f& position);

    void render(sf::RenderWindow& window) override;
    bool isPlayerClose(const sf::Vector2f& playerPosition) const;

    int getCurrentWaypoint() const;
    void setCurrentWaypoint(int waypoint);

    sf::Color getColor() const;
    void setInteractPosition(const sf::Vector2f& position);
    void setAnimationOpposing(int animationIndex);
    void setInteract(bool showInteract);

    sf::Sprite& getSprite();

protected:
    sf::CircleShape mShape;
    float mInteractionRadius;
    sf::CircleShape mInteractCircle;
    sf::Text mInteractText;
    bool mShowInteract;
    sf::Font mFont;

    std::map<std::string, Dialogue> mDialogues;
    std::string mActiveDialogueKey;
    DialogueManager mDialogueManager;

    bool mPaused;
    float mSpeed;
    std::vector<sf::Vector2f> mPath;
    size_t mCurrentWaypoint;

    bool mIsPaused;
    float mPauseDuration;
    float mPauseTimer;
    std::chrono::steady_clock::time_point mPauseStartTime;
    int mPriority;

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
    sf::Sprite mSprite;
    Animation mAnimations[int(AnimationIndex::Count)];
    AnimationIndex mCurrentAnimation = AnimationIndex::IdleDown;

    sf::Sprite mIconSprite;
    Animation mIconTexture;

    void startPause(float duration);
    void updatePause(float dt);
};

