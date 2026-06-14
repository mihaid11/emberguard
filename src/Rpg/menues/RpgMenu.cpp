#include "RpgMenu.h"
#include <iostream>
#include "../gamengine/RPGEngine.h"
#include "../../core/GameManager.h"

RpgMenu::RpgMenu(const sf::Vector2f& windowSize, SkillTree& skillTree, Inventory& inventory,
           RPGEngine& rpgEngine, GameManager* gameManager, int& crystals)
    : Menu(windowSize), mCurrentMenu("Inventory"), mGameManager(gameManager), mGap(0), mCrystals(crystals),
    mInventoryButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "Inventory"),
    mSkillTreeButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "SkillTree"),
    mExitButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "Exit"), mShowText(false) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font" << std::endl;

    sf::Vector2f subMenuPos(mMenuShape.getPosition().x + 10.f, mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y + 20.f);
    sf::Vector2f subMenuSize(mMenuShape.getSize().x - 20.f, mMenuShape.getSize().y - mHoveredZoneShape.getSize().y - 40.f);

    mSkillTreeMenu = std::make_unique<SkillTreeMenu>(subMenuPos, subMenuSize, skillTree);

    float slotSize = mMenuShape.getSize().y * 0.195f;
    mInventoryMenu = std::make_unique<InventoryMenu>(subMenuPos, subMenuSize, inventory,
                                                     rpgEngine.getPlayer(), sf::Vector2f(slotSize, slotSize));

    mErrorText.setFillColor(sf::Color::White);
    mErrorText.setFont(mFont);
    mErrorText.setCharacterSize(18);
    mErrorText.setString("Skill Menu is not yet implemented!");
    mErrorText.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.05f,
                                        mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.2f));

    sf::Vector2f buttonSize(mHoveredZoneShape.getSize().x * 0.12f, mHoveredZoneShape.getSize().y * 0.6f);
    mInventoryButton.setSize(buttonSize);
    mSkillTreeButton.setSize(buttonSize);
    mExitButton.setSize(buttonSize);

    mGap = mHoveredZoneShape.getSize().x * 0.04f;;
    float startX = mHoveredZoneShape.getPosition().x + mGap;
    float startY = mHoveredZoneShape.getPosition().y + (mHoveredZoneShape.getSize().y - buttonSize.y) / 2.f;

    mInventoryButton.setPosition(sf::Vector2f(startX, startY));
    mSkillTreeButton.setPosition(sf::Vector2f(startX + buttonSize.x + mGap, startY));
    mExitButton.setPosition(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x - buttonSize.x - mGap, startY));

    mInventoryButton.setCallback([&]() {
        switchToMenu("Inventory");
    });

    mSkillTreeButton.setCallback([&]() {
        // TODO : Implement prototype of skill tree menu
        // For now only an error message is displayed
        //switchToMenu("SkillTree");
        mErrorText.setFillColor(sf::Color::White);
        mClock.restart();
        mShowText = true;
    });

    mExitButton.setCallback([&]() {
        rpgEngine.saveGame();
        if (mGameManager)
            mGameManager->switchToMainMenu();
        else
            std::cerr << "Error: GameManager is nullptr in exitButton callback." << std::endl;
    });

    mButtons.push_back(&mInventoryButton);
    mButtons.push_back(&mSkillTreeButton);
    mButtons.push_back(&mExitButton);

    mCrystalText.setFont(mFont);
    mCrystalText.setCharacterSize(13);
    mCrystalText.setFillColor(sf::Color::White);
    mCrystalText.setPosition(mMenuShape.getPosition().x + mMenuShape.getSize().x - mCrystalText.getGlobalBounds().width - mExitButton.getSize().x - mGap * 1.6f,
                             mMenuShape.getPosition().y + (mHoveredZoneShape.getSize().y - mCrystalText.getGlobalBounds().height) / 2.f);
}

void RpgMenu::handleMouseClick(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons) {
        if (button->isMouseOver(mousePos))
            button->onClick();
    }

    if (mCurrentMenu == "SkillTree")
        mSkillTreeMenu->handleMouseClick(mousePos);

    if (mCurrentMenu == "Inventory")
        mInventoryMenu->handleMouseClick(mousePos);
}

void RpgMenu::updateHover(const sf::Vector2f& mousePos) {
    if (!mIsActive)
        return;

    for (auto& button : mButtons)
        button->updateHover(mousePos);

    if (mCurrentMenu == "SkillTree")
        mSkillTreeMenu->updateHover(mousePos);

    if (mCurrentMenu == "Inventory")
        mInventoryMenu->updateHover(mousePos);
}

void RpgMenu::switchToMenu(const std::string& menuName) {
    mCurrentMenu = menuName;
}

void RpgMenu::restart() {
    mInventoryMenu->restart();
}

void RpgMenu::update(float dt) {
    if (!mIsActive)
        return;

    mCrystalText.setString("Crystals " + std::to_string(mCrystals));
    mCrystalText.setPosition(mMenuShape.getPosition().x + mMenuShape.getSize().x - mCrystalText.getGlobalBounds().width - mExitButton.getSize().x - mGap * 1.6f,
                             mMenuShape.getPosition().y + (mHoveredZoneShape.getSize().y - mCrystalText.getGlobalBounds().height) / 2.f);

    if(mCurrentMenu == "Inventory")
        mInventoryMenu->update(dt);
    else if(mCurrentMenu == "SkillTree")
        mSkillTreeMenu->update(dt);
}

const std::string& RpgMenu::getMenuType() const {
    return mCurrentMenu;
}

InventoryMenu& RpgMenu::getInventoryMenu() {
    return *mInventoryMenu;
}

void RpgMenu::render(sf::RenderWindow& window) {
    if (!mIsActive)
        return;

    Menu::render(window);
    window.draw(mCrystalText);

    for (auto& button : mButtons)
        button->render(window);

    if (mCurrentMenu == "SkillTree")
        mSkillTreeMenu->render(window);
    else if (mCurrentMenu == "Inventory")
        mInventoryMenu->render(window);

    // If the error text is visible gradually make it dissapear and render it
    if (mShowText) {
        float elapsedTime = mClock.getElapsedTime().asSeconds();
        if (elapsedTime > 1.8f) {
            mShowText = false;
            mErrorText.setString("");
        } else {
            int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 1.8f)));
            mErrorText.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        window.draw(mErrorText);
    }
}
