#include "TransitionSystem.h"
#include <math.h>

TransitionSystem::TransitionSystem(const sf::Vector2f& windowSize) 
    : mWindowSize(windowSize),
      mIsActive(false),
      mIsReversing(false),
      mIsHolding(false),
      mProgress(0.0f) {
    
    mCircularMask.setRadius(1.f);
    mCircularMask.setOrigin(1.f, 1.f);
    mCircularMask.setFillColor(sf::Color::Black);
    mCircularMask.setPointCount(64);
}

void TransitionSystem::startSleepTransition(
    const sf::Vector2f& center,
    float durationSec,
    std::function<bool()> shouldHold,
    std::function<void()> onWake
) {
    mCenter = center;
    mDuration = durationSec;
    mProgress = 0.0f;
    mIsActive = true;
    mIsReversing = false;
    mIsHolding = false;
    mShouldHold = shouldHold;
    mOnWake = onWake;

    mCircularMask.setRadius(1.f);
    mCircularMask.setOrigin(1.f, 1.f);
    mCircularMask.setPosition(mCenter);
}

void TransitionSystem::update(float deltaTime) {
    if (!mIsActive) return;

    if (!mIsReversing && !mIsHolding) {
        mProgress += deltaTime / mDuration;
        
        if (mProgress >= 1.0f) {
            mProgress = 1.0f;
            mIsHolding = true;
        }
    } else if (mIsHolding) {
        if (mShouldHold && !mShouldHold()) {
            mIsHolding = false;
            mIsReversing = true;
            mProgress = 0.0f;
            if (mOnWake) mOnWake();
        }
    } else if (mIsReversing) {
        mProgress += deltaTime / mDuration;
        
        if (mProgress >= 1.0f) {
            mProgress = 1.0f;
            mIsActive = false;
        }
    }

    updateCircularVignette();
}

void TransitionSystem::updateCircularVignette() {
    float progress = mIsReversing ? (1.0f - mProgress) : mProgress;
    
    float distances[] = {
        mCenter.x,
        mWindowSize.x - mCenter.x,
        mCenter.y,
        mWindowSize.y - mCenter.y
    };
    
    float maxRadius = *std::max_element(distances, distances + 4) * 1.4f;
    
    float currentRadius = progress * maxRadius;
    mCircularMask.setRadius(currentRadius);
    mCircularMask.setOrigin(currentRadius, currentRadius);
    mCircularMask.setPosition(mCenter);
}

void TransitionSystem::render(sf::RenderWindow& window) {
    if (!mIsActive) return;

    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());

    window.draw(mCircularMask, sf::BlendAlpha);
    
    window.setView(currentView);
}

bool TransitionSystem::isTransitioning() const {
    return mIsActive;
}

