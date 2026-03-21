#include "Menu.h"
#include <iostream>
#include "../gamengine/RPGEngine.h"
#include "../../GameManager.h"

Menu::Menu(sf::RenderWindow& window, SkillTree& skillTree, Inventory& inventory,
           RPGEngine& rpgEngine, GameManager* gameManager)
    : mCurrentMenu("Inventory"), mGameManager(gameManager), mGap(0),
    mInventoryButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "Inventory"),
    mSkillTreeButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "SkillTree"),
    mExitButton(sf::Vector2f(0, 0), sf::Vector2f(80.0f, 35.0f), "Exit"), mShowText(false) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font" << std::endl;

    mBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    mBackground.setFillColor(sf::Color(50, 50, 50, 185));
    mBackground.setPosition(sf::Vector2f(0, 0));

    mMenuShape.setSize(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
    mMenuShape.setFillColor(sf::Color(50, 50, 50, 255));
    mMenuShape.setPosition((window.getSize().x - mMenuShape.getSize().x) / 2.0f,
                           (window.getSize().y - mMenuShape.getSize().y) / 2.0f);

    mHoveredZoneShape.setSize(sf::Vector2f(mMenuShape.getSize().x, mMenuShape.getSize().y * 0.155f));
    mHoveredZoneShape.setFillColor(sf::Color(10, 10, 10, 100));
    mHoveredZoneShape.setPosition(mMenuShape.getPosition());

    mSkillTreeMenu = std::make_unique<SkillTreeMenu>(sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x * 0.156f, mMenuShape.getPosition().y + mMenuShape.getSize().y * 0.277f),
                                                     sf::Vector2f(mMenuShape.getSize().x * 0.968f, mMenuShape.getSize().y * 0.694f), skillTree);
    float slotSize = mMenuShape.getSize().y * 0.195f;
    mInventoryMenu = std::make_unique<InventoryMenu>(inventory,
                                                     sf::Vector2f(mMenuShape.getPosition().x + mMenuShape.getSize().x / 2.f,
                                                                  mMenuShape.getPosition().y + mHoveredZoneShape.getSize().y +
                                                                  (mMenuShape.getSize().y - mHoveredZoneShape.getSize().y) / 4.f),
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

    mButtons.push_back(mInventoryButton);
    mButtons.push_back(mSkillTreeButton);
    mButtons.push_back(mExitButton);

    mCrystalText.setFont(mFont);
    mCrystalText.setCharacterSize(13);
    mCrystalText.setFillColor(sf::Color::White);
    mCrystalText.setPosition(mMenuShape.getPosition().x + mMenuShape.getSize().x - mCrystalText.getGlobalBounds().width - mExitButton.getSize().x - mGap * 1.6f,
                             mMenuShape.getPosition().y + (mHoveredZoneShape.getSize().y - mCrystalText.getGlobalBounds().height) / 2.f);
}

void Menu::handleMouseClick(const sf::Vector2f& mousePos)
{
    for (auto& button : mButtons) {
        if (button.isMouseOver(mousePos))
            button.onClick();
    }

    if (mCurrentMenu == "SkillTree")
        mSkillTreeMenu->handleMouseClick(mousePos);

    if (mCurrentMenu == "Inventory")
        mInventoryMenu->handleMouseClick(mousePos);
}

void Menu::updateHover(const sf::Vector2f& mousePos) {
    for (auto& button : mButtons)
        button.updateHover(mousePos);

    if (mCurrentMenu == "SkillTree")
        mSkillTreeMenu->updateHover(mousePos);

    if (mCurrentMenu == "Inventory")
        mInventoryMenu->updateHover(mousePos);
}

void Menu::switchToMenu(const std::string& menuName) {
    mCurrentMenu = menuName;
    std::cout << "Switched to " << menuName << " menu." << std::endl;
}

void Menu::restart() {
    mInventoryMenu->restart();
}

void Menu::update(int crystals, const Inventory& inventory, const SkillTree& skillTree) {
    mCrystalText.setString("Crystals " + std::to_string(crystals));
    mCrystalText.setPosition(mMenuShape.getPosition().x + mMenuShape.getSize().x - mCrystalText.getGlobalBounds().width - mExitButton.getSize().x - mGap * 1.6f,
                             mMenuShape.getPosition().y + (mHoveredZoneShape.getSize().y - mCrystalText.getGlobalBounds().height) / 2.f);

    mSkillTreeButton.setCallback([&]() {
        // TODO : Implement prototype of skill tree menu
        // For now only an error message is displayed
        //switchToMenu("SkillTree");
        mErrorText.setString("Skill Menu is not yet implemented!");
        mErrorText.setFillColor(sf::Color::White);
        mClock.restart();
        mShowText = true;
    });

    if(mCurrentMenu == "Inventory")
        mInventoryMenu->update();
    else if(mCurrentMenu == "SkillTree")
        mSkillTreeMenu->update(skillTree);
}

const std::string& Menu::getMenuType() const {
    return mCurrentMenu;
}

InventoryMenu& Menu::getInventoryMenu() {
    return *mInventoryMenu;
}

void Menu::render(sf::RenderWindow& window) {
    window.draw(mBackground);
    window.draw(mMenuShape);
    window.draw(mHoveredZoneShape);
    window.draw(mCrystalText);
    for (auto& button : mButtons)
        button.render(window);

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

