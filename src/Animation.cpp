#include "Animation.h"
#include <iostream>

Animation::Animation(int x, int y, int height, int width, const std::string fileName, int numFrames, float interval)
    : mNumFrames(numFrames), mInterval(interval)
{
    if (!mTexture.loadFromFile(fileName))
        std::cout << "Couldn't load texture for main character!" << std::endl;

    for (int i = 0; i < mNumFrames; ++i)
    {
        mFrames[i] = { x + i * width, y, width, height };
    }
}

void Animation::applyToSprite(sf::Sprite& sprite) const
{
    sprite.setTexture(mTexture);
    sprite.setTextureRect(mFrames[mIFrame]);
}

void Animation::update(float dt)
{
    mTime += dt / mInterval;
    while (mTime >= mHoldTime) {
        mTime -= mHoldTime;
        advance();
    }
}

void Animation::advance()
{
    if (++mIFrame >= mNumFrames)
        mIFrame = 0;
}
