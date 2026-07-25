#pragma once
#include "SFML/Graphics.hpp"
#include "../../ui/Menu.h"
#include "../../ui/Button.h"
#include "../inventory/Inventory.h"
#include "../inventory/items/Item.h"
#include "../inventory/items/TowerBlueprint.h"
#include "../inventory/items/TowerBlueprintRare.h"
#include "../inventory/items/TowerBlueprintEpic.h"
#include "../inventory/items/TowerBlueprintMythic.h"
#include "../../core/TimeSystem.h"

class GameManager;

class AnalyzeMenu : public Menu {
public:
    AnalyzeMenu(const sf::Vector2f& windowSize, Inventory& inventory, TimeSystem& timeSystem, GameManager* gameManager,
                std::vector<int>& availableTowers, const sf::Vector2f& slotSize, int& crystals);

    void render(sf::RenderWindow& window) override;
    void handleMouseClick(const sf::Vector2f& mousePos) override;
    void updateHover(const sf::Vector2f& mousePos) override;
    void update(float dt) override;

    void getInfo(int& extracting, int& inSlot, int& completed, int& timerActive, int& startYear, int& startDay,
                 int& startHour, int& startMinute, int& slotItemId);
    void setInfo(int extracting, int inSlot, int completed, int timerActive, int startYear, int startDay,
                 int startHour, int startMinute, int slotItemId);
    void reset();

private:
    Inventory& mInventory;
    TimeSystem& mTimeSystem;
    GameManager* mGameManager;

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
    sf::Text mTimerText;

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
