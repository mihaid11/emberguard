#include "ChestMenu.h"
#include <iostream>

ChestMenu::ChestMenu(sf::RenderWindow& window, Inventory& inventory, Inventory& chestInventory, const sf::Vector2f& slotSize)
    : mInventory(inventory), mChestInventory(chestInventory), mHoveredSlot(-1) {
    
    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font from file" << std::endl;

    mMenuShape.setSize(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition((window.getSize().x - mMenuShape.getSize().x) / 2.f,
                           (window.getSize().y - mMenuShape.getSize().y) / 2.f);

    mHoveredZoneShape.setSize(sf::Vector2f(mMenuShape.getSize().x, 40));
    mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
    mHoveredZoneShape.setPosition(mMenuShape.getPosition());

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
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);
    window.draw(mInventoryText);
    window.draw(mChestText);

    for (int i = 0; i < mInventorySlots.size(); ++i) {
        if (i == mHoveredSlot)
            mInventorySlots[i].setFillColor(sf::Color(120, 120, 120, 210));
        else
            mInventorySlots[i].setFillColor(sf::Color(0, 0, 0, 220));

        window.draw(mInventorySlots[i]);

        const Item* item = mInventory.getItemAt(i);
        if (item) {
            sf::RectangleShape icon = item->getIcon();
            icon.setPosition(mInventorySlots[i].getPosition());
            window.draw(icon);
        }
    }

    for (int i = 0; i < mChestSlots.size(); ++i) {
        if (i + mInventorySlots.size() == mHoveredSlot)
            mChestSlots[i].setFillColor(sf::Color(120, 120, 120, 210));
        else
            mChestSlots[i].setFillColor(sf::Color(0, 0, 0, 220));

        window.draw(mChestSlots[i]);

        const Item* item = mChestInventory.getItemAt(i);
        if (item) {
            sf::RectangleShape icon = item->getIcon();
            icon.setPosition(mChestSlots[i].getPosition());
            window.draw(icon);
        }
    }

    if (mHoveredSlot != -1) {
        const Item* item = nullptr;
        if (mHoveredSlot < mInventorySlots.size()) {
            item = mInventory.getItemAt(mHoveredSlot);
        } else {
            int chestSlot = mHoveredSlot - mInventorySlots.size();
            if (chestSlot < mChestSlots.size()) {
                item = mChestInventory.getItemAt(chestSlot);
            }
        }

        if (item) {
            std::string quantityText = "";
            if (mHoveredSlot < mInventorySlots.size()) {
                quantityText = "   " + std::to_string(mInventory.getItemQuantityAt(mHoveredSlot));
            } else {
                int chestSlot = mHoveredSlot - mInventorySlots.size();
                quantityText = "   " + std::to_string(mChestInventory.getItemQuantityAt(chestSlot));
            }
            
            mTooltipText.setString(item->getName() + quantityText + "\n" + item->getDescription());
            
            sf::Vector2f tooltipPos;
            if (mHoveredSlot < mInventorySlots.size()) {
                tooltipPos = mInventorySlots[mHoveredSlot].getPosition();
            } else {
                int chestSlot = mHoveredSlot - mInventorySlots.size();
                tooltipPos = mChestSlots[chestSlot].getPosition();
            }
            
            mTooltipText.setPosition(tooltipPos.x, tooltipPos.y - mTooltipText.getLocalBounds().height - 5);
            mTooltipBackground.setSize(sf::Vector2f(
                mTooltipText.getLocalBounds().width + 10,
                mTooltipText.getLocalBounds().height + 10
            ));
            mTooltipBackground.setPosition(mTooltipText.getPosition().x - 5, mTooltipText.getPosition().y - 5);
            
            window.draw(mTooltipBackground);
            window.draw(mTooltipText);
        }
    }
}

void ChestMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (mHoveredSlot == -1) return;

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
}

void ChestMenu::updateHover(const sf::Vector2f& mousePos) {
    mHoveredSlot = -1;

    for (int i = 0; i < mInventorySlots.size(); ++i) {
        if (mInventorySlots[i].getGlobalBounds().contains(mousePos)) {
            mHoveredSlot = i;
            return;
        }
    }
    
    // Check chest slots (6-9)
    for (int i = 0; i < mChestSlots.size(); ++i) {
        if (mChestSlots[i].getGlobalBounds().contains(mousePos)) {
            mHoveredSlot = i + mInventorySlots.size();
            return;
        }
    }
}

int ChestMenu::getSlotIndexAtPosition(const sf::Vector2f& pos) const {
    for (int i = 0; i < mInventorySlots.size(); ++i)
        if (mInventorySlots[i].getGlobalBounds().contains(pos))
            return i;

    for (int i = 0; i < mChestSlots.size(); ++i) {
        if (mChestSlots[i].getGlobalBounds().contains(pos)) {
            return i + mInventorySlots.size();
        }
    }
    
    return -1;
}

