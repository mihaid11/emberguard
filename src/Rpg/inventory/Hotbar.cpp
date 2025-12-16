#include "Hotbar.h"
#include <iostream>

Hotbar::Hotbar(Inventory& inventory, const sf::Vector2f& position, int slotCount, const sf::Vector2f& slotSize)
    : mInventory(inventory), mPosition(position), mSlotCount(slotCount), mHoveredSlot(-1), mSlotSize(slotSize), mShowText(false) {
    mSlots.resize(3);
    for (int i = 0; i < 3; ++i) {
        mSlots[i].setSize(sf::Vector2f(slotSize.x, slotSize.y));
        mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mSlots[i].setOutlineColor(sf::Color::White);
        mSlots[i].setOutlineThickness(2.f);
        mSlots[i].setPosition(mPosition.x + i * slotSize.x, mPosition.y);
    }

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in the hotbar menu!";

    mQuantityText.setFont(mFont);
    mQuantityText.setCharacterSize(10);
    mQuantityText.setFillColor(sf::Color::White);

    mItemNameText.setFont(mFont);
    mItemNameText.setCharacterSize(12);
    mItemNameText.setFillColor(sf::Color::White);
}

void Hotbar::update() {
    updateSlotColors();
}

void Hotbar::render(sf::RenderWindow& window) {
    for (int i = 0; i < 3; ++i) {
        window.draw(mSlots[i]);

        int quantity = mInventory.getItemQuantityAt(i);
        mQuantityText.setString(std::to_string(quantity));
        mQuantityText.setPosition(sf::Vector2f(mSlots[i].getPosition().x + mSlots[i].getSize().x - mQuantityText.getGlobalBounds().width - 5.f,
            mSlots[i].getPosition().y + mSlots[i].getSize().y - mQuantityText.getGlobalBounds().height - 2.f));

        if (mInventory.getItemAt(i)) {
            sf::RectangleShape icon = mInventory.getItemAt(i)->getIcon();
            icon.setPosition(mSlots[i].getPosition());
            icon.setScale(sf::Vector2f(0.5f, 0.5f));

            window.draw(icon);
            window.draw(mQuantityText);
        }

        if (mShowText) {
            float elapsedTime = mClock.getElapsedTime().asSeconds();
            if (elapsedTime > 2.5f) {
                mShowText = false;
                mItemNameText.setString("");
            } else {
                int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 2.5f)));
                mItemNameText.setFillColor(sf::Color(255, 255, 255, alpha));
            }
            window.draw(mItemNameText);
        }
    }
}

int Hotbar::getHoveredSlot() const {
    return mHoveredSlot;
}

void Hotbar::setHoveredSlot(int slot) {
    if(mInventory.getItemAt(slot)) {
        if (mHoveredSlot == slot) {
            mHoveredSlot = -1;
            mShowText = false;
        } else {
            mHoveredSlot = slot;

            mItemNameText.setString(mInventory.getItemAt(slot)->getName());
            mItemNameText.setPosition(sf::Vector2f(mSlots[mHoveredSlot].getPosition().x + mSlots[mHoveredSlot].getSize().x / 2 - mItemNameText.getGlobalBounds().width / 2,
                mSlots[mHoveredSlot].getPosition().y - mSlotSize.y / 1.8f));
            mItemNameText.setFillColor(sf::Color::White);
            mClock.restart();
            mShowText = true;

        }
    }

}

void Hotbar::updateSlotColors() {
    for (int i = 0; i < 3; ++i) {
        if (mInventory.getItemAt(i)) {
            mSlots[i].setFillColor(i == mHoveredSlot ? sf::Color(120, 120, 120, 210) : sf::Color(0, 0, 0, 220));
        } else {
            mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
            if (mHoveredSlot == i)
                mHoveredSlot = -1;
        }
    }
}

int Hotbar::contains(const sf::Vector2f& mousePos) {
    for(int i = 0; i < 3; ++i) {
        if((mousePos.x > mSlots[i].getPosition().x && mousePos.x < (mSlots[i].getPosition().x + mSlotSize.x)) &&
            (mousePos.y > mSlots[i].getPosition().y && mousePos.y < (mSlots[i].getPosition().y + mSlotSize.y)))
            return i;
    }
    return -1;
}

