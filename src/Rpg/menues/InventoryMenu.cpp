#include "InventoryMenu.h"
#include <iostream>
#include <string>

InventoryMenu::InventoryMenu(const sf::Vector2f& position, const sf::Vector2f& size, Inventory& inventory,
                             MainCharacter& character, const sf::Vector2f& slotSize)
    : Menu(size, position, true), mCharacter(character), mInventory(inventory), mSlotSize(slotSize), mShowTooltip(false),
    mGap(mSlotSize.x * 2.f), mHoveredSlot(-1), mDraggedSlot(-1), mCharacterSprite(mCharacter.getAvatarSprite()) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in the inventory menu!";

    mLevelText.setFont(mFont);
    mLevelText.setCharacterSize(15);
    mLevelText.setFillColor(sf::Color::White);

    mXpBar.setFillColor(sf::Color(50, 50, 50, 200));
    mXpBar.setOutlineColor(sf::Color::White);
    mXpBar.setOutlineThickness(1.f);

    mXpBarFill.setFillColor(sf::Color::Cyan);

    mXpText.setFont(mFont);
    mXpText.setCharacterSize(12);
    mXpText.setFillColor(sf::Color::White);

    mTooltipText.setFont(mFont);
    mTooltipText.setCharacterSize(14);
    mTooltipText.setFillColor(sf::Color::White);

    mTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200));
    mTooltipBackground.setOutlineColor(sf::Color::White);
    mTooltipBackground.setOutlineThickness(1.0f);

    initializeLayout();
    mIsActive = true;
}

void InventoryMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    window.draw(mMenuShape);

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

    if (mShowTooltip && mHoveredSlot != -1) {
        window.draw(mTooltipBackground);
        window.draw(mTooltipText);
    }
}

void InventoryMenu::update(float dt) {
    if (!mIsActive)
        return;

    mLevelText.setString("Level " + std::to_string(mCharacter.getLevel()));
    mXpText.setString(std::to_string(mCharacter.getXp()) + " / " + std::to_string(mCharacter.getXpForNextLevel()));

    mXpText.setPosition(mXpBar.getPosition().x + (mXpBar.getSize().x - mXpText.getLocalBounds().width) / 2.f, mXpText.getPosition().y);
    mXpBarFill.setSize(sf::Vector2f(mXpBar.getSize().x * mCharacter.getXpPercentage(), mXpBar.getSize().y));
    mLevelText.setPosition(mXpBar.getPosition().x + (mXpBar.getSize().x - mLevelText.getLocalBounds().width) / 2.f, mLevelText.getPosition().y);
}

void InventoryMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    int lastHoveredSlot = mHoveredSlot;
    mHoveredSlot = getSlotIndexAtPosition(mousePos);

    if (mHoveredSlot != lastHoveredSlot) {
        updateSlotColors();
        updateTooltip();
    }
}

void InventoryMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    if (mHoveredSlot != -1) {
        if (mDraggedSlot == -1) {
            if (mInventory.getItemAt(mHoveredSlot)) {
                mDraggedSlot = mHoveredSlot;
                updateSlotColors();
            }
        } else {
            mInventory.swapItems(mDraggedSlot, mHoveredSlot);
            mDraggedSlot = -1;
            updateSlotColors();
            updateTooltip();
        }
    }
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

void InventoryMenu::restart() {
    mHoveredSlot = -1;
    mDraggedSlot = -1;
    updateSlotColors();
}

int InventoryMenu::getHoveredSlot() const {
    return mHoveredSlot;
}

int InventoryMenu::getSlotIndexAtPosition(const sf::Vector2f& pos) const {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (mSlots[i].getGlobalBounds().contains(pos))
            return i;
    }
    return -1;
}

void InventoryMenu::updateTooltip() {
    if (mHoveredSlot == -1) {
        mShowTooltip = false;
        return;
    }

    const Item* item = mInventory.getItemAt(mHoveredSlot);
    if (item) { // Check if item exists
        mTooltipText.setString(item->getName() + "   " + std::to_string(mInventory.getItemQuantityAt(mHoveredSlot))
            + "\n" + item->getDescription());

        sf::FloatRect textBounds = mTooltipText.getGlobalBounds();
        mTooltipBackground.setSize(sf::Vector2f(textBounds.width + 10.f, textBounds.height + 10.f));

        sf::Vector2f slotPosition = mSlots[mHoveredSlot].getPosition();
        mTooltipText.setPosition(slotPosition.x + 10.f, slotPosition.y - mTooltipBackground.getGlobalBounds().height - 5.f);
        mTooltipBackground.setPosition(mTooltipText.getPosition().x - 5.f, mTooltipText.getPosition().y - 5.f);

        mShowTooltip = true;
    } else {
        mShowTooltip = false;
    }
}

void InventoryMenu::initializeLayout() {
    int totalSlots = mInventory.getSlotCount();
    mSlots.resize(totalSlots);

    sf::FloatRect spriteBounds = mCharacterSprite.getGlobalBounds();
    float width = spriteBounds.width + mInventory.getCols() * mSlotSize.x + mGap;

    float startX = mMenuShape.getPosition().x + (mMenuShape.getSize().x - width) / 2.f;
    float startY = mMenuShape.getPosition().y + (mMenuShape.getSize().y - mInventory.getRows() * mSlotSize.y) / 2.f - 15.f;
    mCharacterSprite.setPosition(startX + spriteBounds.width / 2.f, startY + spriteBounds.height / 3.f);

    for (int i = 0; i < totalSlots; ++i) {
        mSlots[i].setSize(mSlotSize);
        mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mSlots[i].setOutlineColor(sf::Color::White);
        mSlots[i].setOutlineThickness(2.0f);

        int row = i / mInventory.getCols();
        int col = i % mInventory.getCols();
        mSlots[i].setPosition(startX + spriteBounds.width + mGap + col * mSlotSize.x, startY + row * mSlotSize.y);
    }

    mXpBar.setSize(sf::Vector2f(spriteBounds.width * 2.f, 12.f));
    mXpBar.setPosition(startX + (spriteBounds.width - mXpBar.getSize().x) / 2.f, startY + spriteBounds.height + 7.f);
    mXpBarFill.setPosition(mXpBar.getPosition());
    mXpText.setPosition(startX, mXpBar.getPosition().y + mXpBar.getSize().y + 15.f);
    mLevelText.setPosition(startX, mXpText.getPosition().y + 35.f);
}
