#include "ShopMenu.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include "../../core/GameManager.h"
#include "../../core/quests/QuestTypes.h"

ShopMenu::ShopMenu(const sf::Vector2f& windowSize, Inventory& inventory, TimeSystem& timeSystem,
                   int numItems, int& crystals, GameManager* gameManager)
    : Menu(windowSize), mInventory(inventory), mTimeSystem(timeSystem), mGameManager(gameManager), mCrystals(crystals), mNumItems(numItems) {

    initializeTitle("Shop");

    mTooltipText.setFont(mFont);
    mTooltipText.setCharacterSize(14);
    mTooltipText.setFillColor(sf::Color::White);

    mTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200));
    mTooltipBackground.setOutlineColor(sf::Color::White);
    mTooltipBackground.setOutlineThickness(1.0f);

    mCrystalsText.setFont(mFont);
    mCrystalsText.setCharacterSize(18);
    mCrystalsText.setFillColor(sf::Color::White);
    mCrystalsText.setString("Crystals: ");
    mCrystalsText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mCrystalsText.getLocalBounds().width) / 2.f,
                                           mMenuShape.getPosition().y + (mMenuShape.getSize().y - mCrystalsText.getLocalBounds().height) / 1.23f));

    mErrorText.setFillColor(sf::Color::White);
    mErrorText.setFont(mFont);
    mErrorText.setCharacterSize(18);
    mErrorText.setString("Not enough crystals");
    mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.1f,
                                        mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.64f));

    mTimerDisplay.setFont(mFont);
    mTimerDisplay.setCharacterSize(16);
    mTimerDisplay.setFillColor(sf::Color::White);

    int gap = 77.5f;
    sf::Vector2f buttonSize(110.f, 150.f);
    float startY = mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y - buttonSize.y) / 2.4f;

    auto item1Button = std::make_unique<Button>(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - 3 * buttonSize.x) / 2.f - gap, startY), buttonSize, "1");
    auto item2Button = std::make_unique<Button>(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - buttonSize.x) / 2.f, startY), buttonSize, "2");
    auto item3Button = std::make_unique<Button>(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x + buttonSize.x) / 2.f + gap, startY), buttonSize, "3");

    item1Button->setCallback([this]() { buyItem(1); });
    item2Button->setCallback([this]() { buyItem(2); });
    item3Button->setCallback([this]() { buyItem(3); });

    mButtons.push_back(std::move(item1Button));
    mButtons.push_back(std::move(item2Button));
    mButtons.push_back(std::move(item3Button));

    regenerateIds();
}

void ShopMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    Menu::render(window);
    window.draw(mCrystalsText);

    for (auto& button : mButtons)
        button->render(window);

    if (!mTooltipText.getString().isEmpty()) {
        window.draw(mTooltipBackground);
        window.draw(mTooltipText);
    }

    int remainingHours = 23 - mTimeSystem.getHour();
    int remainingMinutes = 59 - mTimeSystem.getMinute();

    std::ostringstream timerText;
    timerText << "Refreshes in: " << remainingHours << "h " << remainingMinutes << "m";

    mTimerDisplay.setString(timerText.str());
    mTimerDisplay.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mTimerDisplay.getLocalBounds().width) / 2.f,
                                           mMenuShape.getPosition().y + (mMenuShape.getSize().y - mTimerDisplay.getLocalBounds().height) / 1.08f));

    window.draw(mTimerDisplay);

    // If the error text is visible gradually make it dissapear and render it
    if (mShowText) {
        float elapsedTime = mClock.getElapsedTime().asSeconds();
        if (elapsedTime > 1.8f) {
            mShowText = false;
            mErrorText.setString("");
        }
        else {
            int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 1.8f)));
            mErrorText.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        window.draw(mErrorText);
    }
}

void ShopMenu::update(float dt) {
    if (!mIsActive)
        return;
}

void ShopMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons)
        if (button->isMouseOver(mousePos))
            button->onClick();
}

void ShopMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    bool hovered = false;
    int ind = 0;
    for (auto& button : mButtons) {
        button->updateHover(mousePos);
        if (button->getIfHovered()) {
            updateTooltip(ind, mItemsId[ind]);
            hovered = true;
        }
        ind++;
    }

    if (!hovered)
        mTooltipText.setString("");
    mCrystalsText.setString("Crystals: " + std::to_string(mCrystals));
    mCrystalsText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + (mMenuShape.getSize().x - mCrystalsText.getLocalBounds().width) / 2.f,
                                           mMenuShape.getPosition().y + (mMenuShape.getSize().y - mCrystalsText.getLocalBounds().height) / 1.23f));
}

void ShopMenu::regenerateIds() {
    mItemsId.erase(mItemsId.begin(), mItemsId.end());
    srand(static_cast<unsigned int>(time(NULL)));
    int item1Id = rand() % (mNumItems + 1);
    mItemsId.push_back(item1Id);
    int item2Id = rand() % (mNumItems + 1);
    mItemsId.push_back(item2Id);
    int item3Id = rand() % (mNumItems + 1);
    mItemsId.push_back(item3Id);
}

void ShopMenu::updateTooltip(int slot, int id) {
    sf::Vector2f basePosition = mMenuShape.getPosition();

    sf::Vector2f slotPosition;
    if (slot == 0) slotPosition = basePosition + sf::Vector2f(60, 120);
    else if (slot == 1) slotPosition = basePosition + sf::Vector2f(210, 120);
    else if (slot == 2) slotPosition = basePosition + sf::Vector2f(360, 120);

    std::string tooltipText;
    if (id == 1) tooltipText = "Cost: 5\nWood\nA piece of wood, useful for crafting.";
    else if (id == 2) tooltipText = "Cost: 50\nTower Blueprint\nCan be used to unlock towers!";
    else if (id == 3) tooltipText = "Cost: 100\nRare Tower Blueprint\nCan be used to unlock rare towers!";
    else if (id == 4) tooltipText = "Cost: 150\nEpic Tower Blueprint\nCan be used to unlock epic towers!";
    else if (id == 5) tooltipText = "Cost: 250\nMythic Tower Blueprint\nCan be used to unlock mythic towers!";
    else tooltipText = "Nothing here!";

    mTooltipText.setString(tooltipText);
    sf::FloatRect textBounds = mTooltipText.getGlobalBounds();

    mTooltipBackground.setSize(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
    mTooltipBackground.setPosition(slotPosition.x + 10, slotPosition.y - textBounds.height - 10);
    mTooltipText.setPosition(mTooltipBackground.getPosition().x + 5, mTooltipBackground.getPosition().y + 5);
}

void ShopMenu::buyItem(int buttonNumber) {
    if (mItemsId[buttonNumber - 1] == 0)
        return;

    mErrorText.setString("Not enough crystals!");
    auto& button = mButtons[buttonNumber - 1];

    mErrorText.setPosition(sf::Vector2f(button->getPosition().x + (button->getSize().x - mErrorText.getLocalBounds().width) / 2.f,
                                        button->getPosition().y + button->getSize().y * 0.75f));

    int itemId = mItemsId[buttonNumber - 1];
    std::unique_ptr<Item> item = nullptr;

    if (itemId == 1)
        item = std::make_unique<Wood>();
    else if (itemId == 2)
        item = std::make_unique<TowerBlueprint>();
    else if (itemId == 3)
        item = std::make_unique<TowerBlueprintRare>();
    else if (itemId == 4)
        item = std::make_unique<TowerBlueprintEpic>();
    else if (itemId == 5)
        item = std::make_unique<TowerBlueprintMythic>();

    if (item) {
        if (item->getPrice() <= mCrystals) {
            mCrystals -= item->getPrice();
            mInventory.addItem(std::move(item), 1);

            if (mGameManager)
                mGameManager->dispatchQuestEvent(GameEvent{ObjectiveType::get_item, "shop_item", 1});
        } else {
            mErrorText.setFillColor(sf::Color::White);
            mClock.restart();
            mShowText = true;
        }
    }
}
