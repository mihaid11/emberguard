#include "ShopMenu.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

ShopMenu::ShopMenu(sf::RenderWindow& window, Inventory& inventory, int numItems, int& crystals)
    : mInventory(inventory), mItem1Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 150.f), "1"),
    mItem2Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 150.f), "2"),
    mItem3Button(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(110.f, 150.f), "3"),
    mCrystals(crystals), mNumItems(numItems) {

    mMenuShape.setSize(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition((window.getSize().x - mMenuShape.getSize().x) / 2.f,
                           (window.getSize().y - mMenuShape.getSize().y) / 2.f);

    mHoveredZoneShape.setSize(sf::Vector2f(window.getSize().x * 3.0f / 4.0f, 40));
    mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
    mHoveredZoneShape.setPosition(sf::Vector2f((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
                                               (window.getSize().y - mMenuShape.getSize().y) / 2.0f));

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in the shop menu!";

    mTooltipText.setFont(mFont);
    mTooltipText.setCharacterSize(14);
    mTooltipText.setFillColor(sf::Color::White);

    mTooltipBackground.setFillColor(sf::Color(50, 50, 50, 200));
    mTooltipBackground.setOutlineColor(sf::Color::White);
    mTooltipBackground.setOutlineThickness(1.0f);

    mShopText.setFont(mFont);
    mShopText.setCharacterSize(20);
    mShopText.setFillColor(sf::Color::White);
    mShopText.setString("Shop");
    mShopText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - 50.f, mMenuShape.getPosition().y + 8.f));

    mCrystalsText.setFont(mFont);
    mCrystalsText.setCharacterSize(18);
    mCrystalsText.setFillColor(sf::Color::White);
    mCrystalsText.setString("Crystals: ");
    mCrystalsText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - 75.f,
                                           mMenuShape.getPosition().y + mMenuShape.getSize().y - 55.f));

    mErrorText.setFillColor(sf::Color::White);
    mErrorText.setFont(mFont);
    mErrorText.setCharacterSize(18);
    mErrorText.setString("Not enought crystals");
    mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.1f,
                                        mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));

    mItem1Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - 225.f,
                                          mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y +
                                         (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y) / 2.f - 75.f));
    mItem2Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f - 55.f,
                                          mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y +
                                         (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y) / 2.f - 75.f));
    mItem3Button.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f + 105.f,
                                          mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y +
                                         (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y) / 2.f - 75.f));

    int item1Id = rand() % (numItems + 1);
    mItemsId.push_back(item1Id);
    int item2Id = rand() % (numItems + 1);
    mItemsId.push_back(item2Id);
    int item3Id = rand() % (numItems + 1);
    mItemsId.push_back(item3Id);

    mItem1Button.setCallback([&]() {
        if (mItemsId[0] == 1) {
            std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
            if (woodItem->getPrice() <= crystals) {
                crystals -= woodItem->getPrice();
                mInventory.addItem(std::move(woodItem), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.08f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[0] == 2) {
            std::unique_ptr<TowerBlueprint> towerBlueprintItem = std::make_unique<TowerBlueprint>();
            if (towerBlueprintItem->getPrice() <= crystals) {
                crystals -= towerBlueprintItem->getPrice();
                mInventory.addItem(std::move(towerBlueprintItem), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.08f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[0] == 3) {
            std::unique_ptr<TowerBlueprintRare> towerBlueprintRareItem = std::make_unique<TowerBlueprintRare>();
            if (towerBlueprintRareItem->getPrice() <= crystals) {
                crystals -= towerBlueprintRareItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintRareItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.08f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[0] == 4) {
            std::unique_ptr<TowerBlueprintEpic> towerBlueprintEpicItem = std::make_unique<TowerBlueprintEpic>();
            if (towerBlueprintEpicItem->getPrice() <= crystals) {
                crystals -= towerBlueprintEpicItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintEpicItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.08f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[0] == 5) {
            std::unique_ptr<TowerBlueprintMythic> towerBlueprintMythicItem = std::make_unique<TowerBlueprintMythic>();
            if (towerBlueprintMythicItem->getPrice() <= crystals) {
                crystals -= towerBlueprintMythicItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintMythicItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.08f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        }
    });

    mItem2Button.setCallback([&]() {
        if (mItemsId[1] == 1) {
            std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
            if (woodItem->getPrice() <= crystals) {
                crystals -= woodItem->getPrice();
                mInventory.addItem(std::move(woodItem), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.34f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[1] == 2) {
            std::unique_ptr<TowerBlueprint> towerBlueprintItem = std::make_unique<TowerBlueprint>();
            if (towerBlueprintItem->getPrice() <= crystals) {
                crystals -= towerBlueprintItem->getPrice();
                mInventory.addItem(std::move(towerBlueprintItem), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.34f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[1] == 3) {
            std::unique_ptr<TowerBlueprintRare> towerBlueprintRareItem = std::make_unique<TowerBlueprintRare>();
            if (towerBlueprintRareItem->getPrice() <= crystals) {
                crystals -= towerBlueprintRareItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintRareItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.34f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[1] == 4) {
            std::unique_ptr<TowerBlueprintEpic> towerBlueprintEpicItem = std::make_unique<TowerBlueprintEpic>();
            if (towerBlueprintEpicItem->getPrice() <= crystals) {
                crystals -= towerBlueprintEpicItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintEpicItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.34f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[1] == 5) {
            std::unique_ptr<TowerBlueprintMythic> towerBlueprintMythicItem = std::make_unique<TowerBlueprintMythic>();
            if (towerBlueprintMythicItem->getPrice() <= crystals) {
                crystals -= towerBlueprintMythicItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintMythicItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.34f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        }
    });
    mItem3Button.setCallback([&]() {
        if (mItemsId[2] == 1) {
            std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
            if (woodItem->getPrice() <= crystals) {
                crystals -= woodItem->getPrice();
                mInventory.addItem(std::move(woodItem), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.595f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[2] == 2) {
            std::unique_ptr<TowerBlueprint> towerBlueprintItem = std::make_unique<TowerBlueprint>();
            if (towerBlueprintItem->getPrice() <= crystals) {
                crystals -= towerBlueprintItem->getPrice();
                mInventory.addItem(std::move(towerBlueprintItem), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.595f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[2] == 3) {
            std::unique_ptr<TowerBlueprintRare> towerBlueprintRareItem = std::make_unique<TowerBlueprintRare>();
            if (towerBlueprintRareItem->getPrice() <= crystals) {
                crystals -= towerBlueprintRareItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintRareItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.595f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[2] == 4) {
            std::unique_ptr<TowerBlueprintEpic> towerBlueprintEpicItem = std::make_unique<TowerBlueprintEpic>();
            if (towerBlueprintEpicItem->getPrice() <= crystals) {
                crystals -= towerBlueprintEpicItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintEpicItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.595f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        } else if (mItemsId[2] == 5) {
            std::unique_ptr<TowerBlueprintMythic> towerBlueprintMythicItem = std::make_unique<TowerBlueprintMythic>();
            if (towerBlueprintMythicItem->getPrice() <= crystals) {
                crystals -= towerBlueprintMythicItem->getPrice();
                mInventory.addItem((std::move(towerBlueprintMythicItem)), 1);
            } else {
                mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.595f,
                                                    mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.665f));
                mErrorText.setString("Not enought crystals!");
                mErrorText.setFillColor(sf::Color::White);
                mClock.restart();
                mShowText = true;
            }
        }
    });

    mButtons.push_back(mItem1Button);
    mButtons.push_back(mItem2Button);
    mButtons.push_back(mItem3Button);
}

void ShopMenu::render(sf::RenderWindow& window) {
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);
    window.draw(mShopText);
    window.draw(mCrystalsText);

    for (auto& button : mButtons)
        button.render(window);

    if (!mTooltipText.getString().isEmpty()) {
        window.draw(mTooltipBackground);
        window.draw(mTooltipText);
    }

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

void ShopMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons)
        if (button.isMouseOver(mousePos))
            button.onClick();
}

void ShopMenu::updateHover(const sf::Vector2f& mousePos) {
    bool hovered = false;
    int ind = 0;
    for (auto& button : mButtons) {
        button.updateHover(mousePos);
        if (button.getIfHovered()) {
            updateTooltip(ind, mItemsId[ind]);
            hovered = true;
        }
        ind++;
    }

    if (!hovered)
        mTooltipText.setString("");
    mCrystalsText.setString("Crystals: " + std::to_string(mCrystals));
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
    else if (id == 3) tooltipText = "Cost: 100\nTower Blueprint Rare\nCan be used to unlock rare towers!";
    else if (id == 4) tooltipText = "Cost: 150\nTower Blueprint Epic\nCan be used to unlock epic towers!";
    else if (id == 5) tooltipText = "Cost: 250\nTower Blueprint Mythic\nCan be used to unlock mythic towers!";
    else tooltipText = "Nothing here!";

    mTooltipText.setString(tooltipText);
    sf::FloatRect textBounds = mTooltipText.getGlobalBounds();

    mTooltipBackground.setSize(sf::Vector2f(textBounds.width + 10, textBounds.height + 10));
    mTooltipBackground.setPosition(slotPosition.x + 10, slotPosition.y - textBounds.height - 10);
    mTooltipText.setPosition(mTooltipBackground.getPosition().x + 5, mTooltipBackground.getPosition().y + 5);
}

