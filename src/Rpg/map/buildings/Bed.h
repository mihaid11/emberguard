#pragma once
#include "../entities/Entity.h"
#include <SFML/Graphics.hpp>
#include "../../../core/TimeSystem.h"
#include "../../../core/TransitionSystem.h"

class GameManager;

class Bed : public Entity {
public:
    Bed(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
        const sf::Vector2f& collPosition, const sf::Vector2f& collSize,
        const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
        TimeSystem& timeSystem, TransitionSystem& transitionSystem,
        GameManager* gameManager);

    void interact() override;
    void render(sf::RenderWindow& window) override;

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

