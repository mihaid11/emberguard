#pragma once
#include "../entities/Entity.h"
#include <SFML/Graphics.hpp>
#include "../../../core/TimeSystem.h"
#include "../../../core/TransitionSystem.h"

class GameManager;

class Bed : public Entity {
public:
    Bed(const sf::Vector2f& position, const std::string filename,
        const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
        TimeSystem& timeSystem, TransitionSystem& transitionSystem,
        GameManager* gameManager, const sf::Vector2f& collPosition = {0.f, 0.f},
        const sf::Vector2f& collSize = {0.f, 0.f});

    void interact() override;
    void render(sf::RenderWindow& window) override;

    float getDepthOffset() const override { return -9000.f; }
private:
    TimeSystem& mTimeSystem;
    TransitionSystem& mTransitionSystem;
    GameManager* mGameManager;

    sf::Vector2f mInteractPosition;
    sf::Vector2f mInteractSize;

    // Rendering of the error message variables
    sf::Font mFont;
    bool mShowText;
    sf::Text mErrorText;
    sf::Clock mClock;
};
