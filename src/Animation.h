#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation() = default;
    Animation(int x, int y, int height, int width, const std::string fileName, int numFrames, float interval);
    void applyToSprite(sf::Sprite& sprite) const;
    void update(float dt);

private:
    int mNumFrames;
    static constexpr float mHoldTime = 0.1f;
    sf::Texture mTexture;
    sf::IntRect mFrames[10];
    int mIFrame = 0;
    float mTime = 0.f;
    float mInterval;

    void advance();
};

