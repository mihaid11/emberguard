#pragma once
#include "SFML/Graphics.hpp"
#include "../inventory/Inventory.h"
#include "../inventory/items/Item.h"
#include "../inventory/items/TowerBlueprint.h"
#include "../inventory/items/TowerBlueprintRare.h"
#include "../inventory/items/TowerBlueprintEpic.h"
#include "../inventory/items/TowerBlueprintMythic.h"
#include "../../TimeSystem.h"
#include "../../TowerDefense/menues/Button.h"

class AnalyzeMenu {
public:
    AnalyzeMenu(sf::RenderWindow& window, Inventory& inventory, TimeSystem& timeSystem,
                std::vector<int>& availableTowers, const sf::Vector2f& slotSize, int& crystals);
    void render(sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void updateHover(const sf::Vector2f& mousePos);
    void update();
    void getInfo(int& extracting, int& inSlot, int& completed, int& timerActive, int& startYear, int& startDay,
                 int& startHour, int& startMinute, int& slotItemId);
    void setInfo(int extracting, int inSlot, int completed, int timerActive, int startYear, int startDay,
                 int startHour, int startMinute, int slotItemId);
    void reset();

private:
    sf::RectangleShape mMenuShape;
    sf::RectangleShape mHoveredZoneShape;
    sf::Text mMenuText;
    sf::Font mFont;

    Inventory& mInventory;
    TimeSystem& mTimeSystem;

    sf::RectangleShape mSlot;
    Item* mSlotItem;
    bool mExtracting;
    // if the item is in slot but the extracting is not active
    bool mInSlot;
    bool mCompleted;
    Button mStartButton;
    Button mCancelButton;
    Button mCompleteButton;

    int mHoveredSlot;
    std::vector<sf::RectangleShape> mSlots;
    sf::RectangleShape mTooltipBackground;
    sf::Text mTooltipText;

    bool mTimerActive;
    int mStartYear;
    int mStartDay;
    int mStartHour;
    int mStartMinute;

    // Rendering of the error message variables
    bool mShowText;
    sf::Text mErrorText;
    sf::Clock mClock;

    int getSlotIndexAtPosition(const sf::Vector2f& pos) const;
    void updateTooltip();
    void updateSlotColors();
};

