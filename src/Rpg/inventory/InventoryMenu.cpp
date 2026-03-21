#include "InventoryMenu.h"
#include <iostream>
#include <string>

InventoryMenu::InventoryMenu(Inventory& inventory, const sf::Vector2f& position,
        MainCharacter& character, const sf::Vector2f& slotSize)
    : mCharacter(character), mInventory(inventory), mPosition(position),
    mSlotSize(slotSize), mGap(mSlotSize.x * 1.5f), mHoveredSlot(-1), mDraggedSlot(-1), mCharacterSprite(mCharacter.getIconSprite()) {

    int totalSlots = mInventory.getSlotCount();
    mSlots.resize(totalSlots);

    sf::FloatRect spriteBounds = mCharacterSprite.getGlobalBounds();
    float width = spriteBounds.width + mInventory.getCols() * mSlotSize.x + mGap;
    mCharacterSprite.setPosition(sf::Vector2f(mPosition.x - width / 2.f, mPosition.y - 21.f));

    for (int i = 0; i < totalSlots; ++i) {
        mSlots[i].setSize(slotSize);
        mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mSlots[i].setOutlineColor(sf::Color::White);
        mSlots[i].setOutlineThickness(2.0f);
        int row = i / mInventory.getCols();
        int col = i % mInventory.getCols();
        mSlots[i].setPosition(mPosition.x - width / 2.f + col * slotSize.x + spriteBounds.width + mGap, mPosition.y + row * slotSize.y);
    }

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in the inventory menu!";

    mLevelText.setFont(mFont);
    mLevelText.setCharacterSize(15);
    mLevelText.setFillColor(sf::Color::White);
    mLevelText.setString("Level 1");
    mLevelText.setPosition(mCharacterSprite.getPosition().x + (spriteBounds.width - mLevelText.getLocalBounds().width) / 2.f,
                           mCharacterSprite.getPosition().y + spriteBounds.height * 1.2f);

    mXpBar.setSize(sf::Vector2f(spriteBounds.width / 1.5f, 12.f));
    mXpBar.setPosition(sf::Vector2f(mCharacterSprite.getPosition().x + (spriteBounds.width - mXpBar.getSize().x) / 2.f,
                                    mLevelText.getPosition().y + mLevelText.getLocalBounds().height + 15.f));
    mXpBar.setFillColor(sf::Color(50, 50, 50, 200));
    mXpBar.setOutlineColor(sf::Color::White);
    mXpBar.setOutlineThickness(1.f);

    mXpBarFill.setSize(sf::Vector2f(0.f, 12.f));
    mXpBarFill.setPosition(mXpBar.getPosition());
    mXpBarFill.setFillColor(sf::Color::Cyan);

    mXpText.setFont(mFont);
    mXpText.setCharacterSize(12);
    mXpText.setFillColor(sf::Color::White);
    mXpText.setString("0 / 50");
    mXpText.setPosition(mCharacterSprite.getPosition().x + (spriteBounds.width - mXpText.getLocalBounds().width) / 2.f,
                        mXpBar.getPosition().y + 20.f);

    mTooltipText.setFont(mFont);
    mTooltipText.setCharacterSize(14);
    mTooltipText.setFillColor(sf::Color::White);

    mTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200));
    mTooltipBackground.setOutlineColor(sf::Color::White);
    mTooltipBackground.setOutlineThickness(1.0f);
}

void InventoryMenu::render(sf::RenderWindow& window) {
    updateSlotColors();

    for (int i = 0; i < mSlots.size(); ++i) {
        window.draw(mSlots[i]);

        const Item* item = mInventory.getItemAt(i);
        if (item) {
            sf::RectangleShape icon = item->getIcon();
            icon.setPosition(mSlots[i].getPosition());
            window.draw(icon);
        }
    }

    window.draw(mCharacterSprite);

    window.draw(mLevelText);
    window.draw(mXpText);
    window.draw(mXpBar);
    window.draw(mXpBarFill);

    if (mHoveredSlot != -1) {
        const Item* item = mInventory.getItemAt(mHoveredSlot);
        if (item) {
            window.draw(mTooltipBackground);
            window.draw(mTooltipText);
        }
    }
}

void InventoryMenu::updateHover(const sf::Vector2f& mousePos) {
    int hoveredSlot = getSlotIndexAtPosition(mousePos);
    if (mInventory.getItemAt(hoveredSlot))
        mHoveredSlot = hoveredSlot;
    else
        mHoveredSlot = -1;
    updateTooltip();
}

void InventoryMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (mHoveredSlot != -1) {
        if (mDraggedSlot == -1) {
            mDraggedSlot = mHoveredSlot;
        } else {
            if(mInventory.getItemAt(mDraggedSlot) && mInventory.getItemAt(mHoveredSlot))
                mInventory.swapItems(mDraggedSlot, mHoveredSlot);
            mDraggedSlot = -1;
        }
    }
}

void InventoryMenu::handleDragAndDrop(const sf::Vector2f& mousePos) {
    if (mDraggedSlot != -1 && mHoveredSlot != -1 && mDraggedSlot != mHoveredSlot) {
        mInventory.swapItems(mDraggedSlot, mHoveredSlot);
        mDraggedSlot = -1;
    }
}

void InventoryMenu::update() {
    int totalSlots = mInventory.getSlotCount();

    if (static_cast<int>(mSlots.size()) != totalSlots)
        mSlots.resize(totalSlots);

    sf::FloatRect spriteBounds = mCharacterSprite.getGlobalBounds();
    float width = spriteBounds.width + mInventory.getCols() * mSlotSize.x + mGap;
    for (int i = 0; i < totalSlots; ++i) {
        int row = i / mInventory.getCols();
        int col = i % mInventory.getCols();
        mSlots[i].setPosition(mPosition.x - width / 2.f + col * mSlotSize.x + spriteBounds.width + mGap,
                              mPosition.y + row * mSlotSize.y);
    }

    mLevelText.setString("Level " + std::to_string(mCharacter.getLevel()));
    mLevelText.setPosition(mCharacterSprite.getPosition().x + (spriteBounds.width - mLevelText.getLocalBounds().width) / 2.f,
                           mCharacterSprite.getPosition().y + spriteBounds.height * 1.2f);

    mXpText.setString(std::to_string(mCharacter.getXp()) + " / " + std::to_string(mCharacter.getXpForNextLevel()));
    mXpText.setPosition(mCharacterSprite.getPosition().x + (spriteBounds.width - mXpText.getLocalBounds().width) / 2.f,
                        mXpBar.getPosition().y + mXpBar.getSize().y + 15.f);

    mXpBarFill.setSize(sf::Vector2f(mXpBar.getSize().x * mCharacter.getXpPercentage(), mXpBar.getSize().y));
}

int InventoryMenu::getHoveredSlot() const {
    return mHoveredSlot;
}

void InventoryMenu::restart() {
    mHoveredSlot = -1;
    mDraggedSlot = -1;
}

void InventoryMenu::updateSlotColors() {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (i == mHoveredSlot) {
            if(i == mDraggedSlot)
                mSlots[i].setFillColor(sf::Color(150, 150, 150, 210));
            else
                mSlots[i].setFillColor(sf::Color(120, 120, 120, 210));
        } else {
            if (i == mDraggedSlot)
                mSlots[i].setFillColor(sf::Color(70, 70, 70, 210));
            else
                mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        }
    }
}

int InventoryMenu::getSlotIndexAtPosition(const sf::Vector2f& pos) const {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (mSlots[i].getGlobalBounds().contains(pos))
            return i;
    }
    return -1;
}

void InventoryMenu::updateTooltip() {
    if (mHoveredSlot != -1) {
        const Item* item = mInventory.getItemAt(mHoveredSlot);
        if (item) { // Check if item exists
            mTooltipText.setString(item->getName() + "   " + std::to_string(mInventory.getItemQuantityAt(mHoveredSlot))
                + "\n" + item->getDescription());

            sf::Vector2f slotPosition = mSlots[mHoveredSlot].getPosition();
            mTooltipText.setPosition(slotPosition.x + 10, slotPosition.y - mTooltipBackground.getGlobalBounds().height - 5);

            sf::FloatRect textBounds = mTooltipText.getGlobalBounds();
            mTooltipBackground.setSize(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
            mTooltipBackground.setPosition(mTooltipText.getPosition().x - 5, mTooltipText.getPosition().y - 5);
        }
    }
}

