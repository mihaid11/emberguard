#include "ChestMenu.h"
#include <iostream>

ChestMenu::ChestMenu(const sf::Vector2f& windowSize, Inventory& inventory, Inventory& chestInventory, const sf::Vector2f& slotSize)
    : Menu(windowSize), mInventory(inventory), mChestInventory(chestInventory), mHoveredSlot(-1) {
    
    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font from file" << std::endl;

    mTitle.setFont(mFont);
    mTitle.setCharacterSize(19);
    mTitle.setFillColor(sf::Color::White);
    mTitle.setString("ChestMenu");

    mTitle.setOrigin(mTitle.getLocalBounds().left + mTitle.getLocalBounds().width / 2.f,
                     mTitle.getLocalBounds().top + mTitle.getLocalBounds().height / 2.f);
    mTitle.setPosition(sf::Vector2f(mHoveredZoneShape.getPosition().x + mHoveredZoneShape.getSize().x / 2.f,
                                    mHoveredZoneShape.getPosition().y + mHoveredZoneShape.getSize().y / 2.f));

    sf::Vector2f mInventoryPosition(mMenuShape.getPosition().x + 90.f, mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + 90.f);
    int totalSlots = mInventory.getSlotCount();
    mInventorySlots.resize(totalSlots);
    for (int i = 0; i < totalSlots; ++i) {
        mInventorySlots[i].setSize(slotSize);
        mInventorySlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mInventorySlots[i].setOutlineColor(sf::Color::White);
        mInventorySlots[i].setOutlineThickness(2.0f);
        int row = i / 3;
        int col = i % 3;
        mInventorySlots[i].setPosition(mInventoryPosition.x + col * slotSize.x,
                                       mInventoryPosition.y + row * slotSize.y);
    }

    sf::Vector2f mChestPosition(mMenuShape.getPosition().x + mMenuShape.getSize().x - 2 * slotSize.x - 90.f, mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + 90.f);
    totalSlots = mChestInventory.getSlotCount();
    mChestSlots.resize(totalSlots);
    for (int i = 0; i < totalSlots; ++i) {
        mChestSlots[i].setSize(slotSize);
        mChestSlots[i].setFillColor(sf::Color(0, 0, 0, 220));
        mChestSlots[i].setOutlineColor(sf::Color::White);
        mChestSlots[i].setOutlineThickness(2.0f);
        int row = i / 2;
        int col = i % 2;
        mChestSlots[i].setPosition(mChestPosition.x + col * slotSize.x,
                                   mChestPosition.y + row * slotSize.y);
    }

    mInventoryText.setFont(mFont);
    mInventoryText.setCharacterSize(20);
    mInventoryText.setFillColor(sf::Color::White);
    mInventoryText.setString("Inventory");
    mInventoryText.setPosition(sf::Vector2f(mInventoryPosition.x + slotSize.x * 3.f / 2.f - mInventoryText.getLocalBounds().width / 2.f,
                                            mInventoryPosition.y - 55.f));

    mChestText.setFont(mFont);
    mChestText.setCharacterSize(20);
    mChestText.setFillColor(sf::Color::White);
    mChestText.setString("Chest");
    mChestText.setPosition(sf::Vector2f(mChestPosition.x + slotSize.x - mChestText.getLocalBounds().width / 2.f,
                                            mChestPosition.y - 55.f));

    mTooltipText.setFont(mFont);
    mTooltipText.setCharacterSize(14);
    mTooltipText.setFillColor(sf::Color::White);

    mTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200));
    mTooltipBackground.setOutlineColor(sf::Color::White);
    mTooltipBackground.setOutlineThickness(1.0f);
}

void ChestMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    Menu::render(window);
    window.draw(mTitle);

    window.draw(mInventoryText);
    window.draw(mChestText);

    drawSlots(window, mInventorySlots, mInventory, 0);
    drawSlots(window, mChestSlots, mChestInventory, mInventorySlots.size());

    if (mShowTooltip && mHoveredSlot != -1) {
        window.draw(mTooltipBackground);
        window.draw(mTooltipText);
    }
}

void ChestMenu::drawSlots(sf::RenderWindow& window, const std::vector<sf::RectangleShape>& slots, Inventory& inventory, int indexOffset) {
    for (int i = 0; i < slots.size(); ++i) {
        if (i + indexOffset == mHoveredSlot)
            const_cast<sf::RectangleShape&>(slots[i]).setFillColor(sf::Color(120, 120, 120, 210));
        else
            const_cast<sf::RectangleShape&>(slots[i]).setFillColor(sf::Color(0, 0, 0, 220));

        window.draw(slots[i]);

        const Item* item = inventory.getItemAt(i);
        if (item) {
            sf::RectangleShape icon = item->getIcon();
            icon.setPosition(slots[i].getPosition());
            window.draw(icon);
        }
    }
}

void ChestMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive || mHoveredSlot == -1)
        return;

    if (mHoveredSlot < mInventorySlots.size()) {
        const Item* item = mInventory.getItemAt(mHoveredSlot);
        if (item) {
            int emptySlot = mChestInventory.getFirstEmptySlot();
            if (emptySlot != -1) {
                mChestInventory.addItem(item->clone(), mInventory.getItemQuantityAt(mHoveredSlot));
                mInventory.removeItemAt(mHoveredSlot);
            }
        }
    }
    else {
        int chestSlot = mHoveredSlot - mInventorySlots.size();
        const Item* item = mChestInventory.getItemAt(chestSlot);
        if (item) {
            int emptySlot = mInventory.getFirstEmptySlot();
            if (emptySlot != -1) {
                mInventory.addItem(item->clone(), mChestInventory.getItemQuantityAt(chestSlot));
                mChestInventory.removeItemAt(chestSlot);
            }
        }
    }

    updateTooltip(mHoveredSlot);
}

void ChestMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    int lastHovered = mHoveredSlot;
    mHoveredSlot = getSlotIndexAtPosition(mousePos);

    if (mHoveredSlot != lastHovered) {
        if (mHoveredSlot != -1)
            updateTooltip(mHoveredSlot);
        else
            mShowTooltip = false;
    }
}

void ChestMenu::update(float dt) {
    if (!mIsActive)
        return;
}

void ChestMenu::updateTooltip(int slotIndex) {
    const Item* item = nullptr;
    int quantity = 0;

    if (slotIndex < mInventorySlots.size()) {
        item = mInventory.getItemAt(slotIndex);
        quantity = mInventory.getItemQuantityAt(slotIndex);
    } else {
        int chestIndex = slotIndex - mInventorySlots.size();
        item = mChestInventory.getItemAt(chestIndex);
        quantity = mChestInventory.getItemQuantityAt(chestIndex);
    }

    if (item) {
        mTooltipText.setString(item->getName() + "   " + std::to_string(quantity) + "\n" + item->getDescription());

        sf::Vector2f tooltipPos;
        if (mHoveredSlot < mInventorySlots.size()) {
            tooltipPos = mInventorySlots[mHoveredSlot].getPosition();
        } else {
            int chestSlot = mHoveredSlot - mInventorySlots.size();
            tooltipPos = mChestSlots[chestSlot].getPosition();
        }

        mTooltipText.setPosition(tooltipPos.x, tooltipPos.y - mTooltipText.getLocalBounds().height - 10.f);
        mTooltipBackground.setSize(sf::Vector2f(mTooltipText.getLocalBounds().width + 10.f,
                                                mTooltipText.getLocalBounds().height + 10.f));
        mTooltipBackground.setPosition(mTooltipText.getPosition().x - 5.f, mTooltipText.getPosition().y - 5.f);

        mShowTooltip = true;
    } else {
        mShowTooltip = false;
    }
}

int ChestMenu::getSlotIndexAtPosition(const sf::Vector2f& pos) const {
    for (int i = 0; i < mInventorySlots.size(); ++i) {
        if (mInventorySlots[i].getGlobalBounds().contains(pos))
            return i;
    }

    for (int i = 0; i < mChestSlots.size(); ++i) {
        if (mChestSlots[i].getGlobalBounds().contains(pos))
            return i + mInventorySlots.size();
    }
    
    return -1;
}
