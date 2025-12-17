#include "Bed.h"
#include <iostream>
#include "../../../GameManager.h"

Bed::Bed(const sf::Vector2f& position, float sizeIncrement, const std::string filename,
         const sf::Vector2f& collPosition, const sf::Vector2f& collSize,
         const sf::Vector2f& interactPosition, const sf::Vector2f& interactSize,
         TimeSystem& timeSystem, TransitionSystem& transitionSystem,
         GameManager* gameManager)
         : Entity(position, filename, collPosition, collSize, 1, interactPosition, interactSize),
        mTimeSystem(timeSystem), mGameManager(gameManager), mInteractSize(interactSize),
        mTransitionSystem(transitionSystem), mInteractPosition(interactPosition) {
    mSprite.setScale(sf::Vector2f(sizeIncrement, sizeIncrement));

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cerr << "Failed to load font for AnalyzeMenu!" << std::endl;

    mErrorText.setFillColor(sf::Color::White);
    mErrorText.setFont(mFont);
    mErrorText.setCharacterSize(11);
    mErrorText.setString("Too early to sleep!");
    mErrorText.setPosition(sf::Vector2f(interactPosition.x + interactSize.x / 2.f - mErrorText.getGlobalBounds().width / 2.f,
                                        interactPosition.y - 40.f));
}

void Bed::interact() {
    if (mTimeSystem.getHour() < 6 || mTimeSystem.getHour() >= 21) {
        mTransitionSystem.startSleepTransition(
            sf::Vector2f(930.f, 300.f),
            1.25f,
            [this]() { return (mTimeSystem.getHour() >= 21 || mTimeSystem.getHour() < 6); },
            [this]() { mTimeSystem.setTimeScale(0.37f); }
        );

        mTimeSystem.setTimeScale(0.000004f);
    } else {
        mErrorText.setPosition(sf::Vector2f(mInteractPosition.x + mInteractSize.x / 2.f - mErrorText.getGlobalBounds().width / 2.f,
                                            mInteractPosition.y - 40.f));
        mErrorText.setString("Too early to sleep!");
        mErrorText.setFillColor(sf::Color::White);
        mClock.restart();
        mShowText = true;
    }
}

void Bed::render(sf::RenderWindow& window) {
    // If the error text is visible gradually make it dissapear and render it
    if (mShowText) {
        float elapsedTime = mClock.getElapsedTime().asSeconds();
        if (elapsedTime > 1.8f) {
            mShowText = false;
            mErrorText.setString("");
        }
        else {
            int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 1.8f)));
            mErrorText.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        window.draw(mErrorText);
    }

    window.draw(mSprite);
    //window.draw(mCollisionZone);
    //window.draw(mInteractableZone);
}

