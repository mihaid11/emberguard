#include "InventoryMenu.h"
#include <iostream>

InventoryMenu::InventoryMenu(Inventory& inventory, const sf::Vector2f& position, const sf::Vector2f& slotSize,
    const sf::Vector2f& playerPos, std::vector<DroppedItem>& droppedItems)
    : mInventory(inventory), mPosition(position), mSlotSize(slotSize), mHoveredSlot(-1), mDraggedSlot(-1),
    mPlayerPos(playerPos), mDroppedItems(droppedItems) {
    int totalSlots = mInventory.getSlotCount();
    mSlots.resize(totalSlots);
    for (int i = 0; i < totalSlots; ++i) {
        mSlots[i].setSize(slotSize);
        mSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mSlots[i].setOutlineColor(sf::Color::White);
        mSlots[i].setOutlineThickness(2.0f);
        int row = i / 3;
        int col = i % 3;
        mSlots[i].setPosition(mPosition.x + col * slotSize.x, mPosition.y + row * slotSize.y);
    }

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in the inventory menu!";

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

void InventoryMenu::update(const Inventory& inventory) {
    mInventory = inventory;
    int totalSlots = mInventory.getSlotCount();

    mSlots.resize(totalSlots);
    for (int i = 0; i < totalSlots; ++i) {
        int row = i / 3;
        int col = i % 3;
        mSlots[i].setPosition(mPosition.x + col * mSlotSize.x, mPosition.y + row * mSlotSize.y);
    }
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

