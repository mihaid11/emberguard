#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class TransitionSystem {
public:
    enum class TransitionType {
        CircularVignette,
        FadeToBlack
    };

    TransitionSystem(const sf::Vector2f& windowSize);
    
    void startSleepTransition(
        const sf::Vector2f& center,
        float durationSec,
        std::function<bool()> shouldHold,
        std::function<void()> onWake
    );
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isTransitioning() const;

private:
    sf::Vector2f mWindowSize;
    bool mIsActive;
    bool mIsReversing;
    bool mIsHolding;
    float mProgress;
    float mDuration;
    
    sf::Vector2f mCenter;
    std::function<bool()> mShouldHold;
    std::function<void()> mOnWake;
    
    sf::CircleShape mCircularMask;
    
    void updateCircularVignette();
};

