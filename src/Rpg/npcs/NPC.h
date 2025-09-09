#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include "../dialogueSystem/DialogueManager.h"
#include "../dialogueSystem/DialogueDatabase.h"
#include "../story/StoryManager.h"
#include "../../Animation.h"
#include "../entities/DrawableEntity.h"

class NPC : public DrawableEntity {
public:
    NPC(const sf::Vector2f& position, const std::string& mId);

    void update(float dt);
    void render(sf::RenderWindow& window) override;

    void setDialogue(const std::string& key, const Dialogue& dialogue);
    void setActiveDialogue(const std::string& key);
    Dialogue getActiveDialogue();
    std::string getCurrentDialogue();
    bool hasMoreDialogue() const;
    void resetDialogue();
    void advanceDialogue();
    void refreshDialogue(const DialogueDatabase& dialogueDatabase, const StoryManager& storyManager);
    void clearDialogue();

    bool hasChoices() const;
    std::vector<DialogueChoice> getChoices() const;
    void selectChoice(int choiceIndex);
    DialogueManager& getDialogueManager();

    void resumeMovement();
    void pauseMovement();
    sf::Vector2f getPosition() const override;
    float getHeight() const override;
    sf::FloatRect getBounds() const;
    std::string getActiveDialogueKey() const;
    void setPosition(const sf::Vector2f& position);

    bool isPlayerClose(const sf::Vector2f& playerPosition) const;

    int getCurrentWaypoint() const;
    void setCurrentWaypoint(int waypoint);

    void setInteractPosition(const sf::Vector2f& position);
    void setAnimationOpposing(int animationIndex);
    void setInteract(bool showInteract);

    std::string getID() const { return mId; }
    sf::Sprite& getSprite();

protected:
    std::string mId;
    float mInteractionRadius;
    sf::CircleShape mInteractCircle;
    sf::Text mInteractText;
    bool mShowInteract;
    sf::Font mFont;

    std::unordered_map<std::string, Dialogue> mDialogues;
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

