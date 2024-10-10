#include "GameEngine.h"
#include "../waves/PathsConfig.h"
#include <iostream>

static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

GameEngine::GameEngine()
    :
    mSpawnTimer(0.0f),
    mTowerHealth(100),
    mCurrentWave(1, 1, { 0 }),
    mCurrentWaveNumber(1),
    mCurrentLevel(1),
    mMap(mCurrentLevel),
    mSelectedTower(nullptr),
    mSelectedEnemy(nullptr),
    mSelectedTowerType(-1),
    mTowerMenu(),
    mCrystals(100),
    mPlayer(sf::Vector2f(100, 100), mProjectiles),
    mPlayerMenu(),
    gameStarted(false),
    mGameOver(false),
    mLevelCompleted(false),
    mIsPaused(false),
    mSmallMenu(mWindow, this, mCurrentLevel),
    mLevelCompleteMenu(mWindow, this, mCurrentLevel),
    mGameOverMenu(mWindow, this, mCurrentLevel),
    mWindow(sf::VideoMode(1280,720), "TowerDefense"){

    if (!mFont.loadFromFile("../assests/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font" << std::endl;

    mCrystalText.setFont(mFont);
    mCrystalText.setPosition(250, 10);
    mCrystalText.setCharacterSize(18);
    mCrystalText.setFillColor(sf::Color::White);

    mHealthBar.setSize(sf::Vector2f(200, 20));
    mHealthBar.setFillColor(sf::Color::Green);
    mHealthBar.setPosition(10, 10);

    mHealthBarBackground.setSize(sf::Vector2f(200, 20));
    mHealthBarBackground.setFillColor(sf::Color::Red);
    mHealthBarBackground.setPosition(10, 10);

    mTowerSelectionMenu = TowerSelectionMenu();
    mTowerMenu = TowerMenu();
    mPlayerMenu.setPosition(sf::Vector2f(500, 7.5f));

    // Initialize the Start Game button
    startGameButton.setSize(sf::Vector2f(150, 30));
    startGameButton.setFillColor(sf::Color(0, 0, 0, 200));
    startGameButton.setPosition(10, 40);
    startGameButton.setOutlineThickness(2);
    startGameButton.setOutlineColor(sf::Color::White);
    startGameButtonText.setFont(mFont);
    startGameButtonText.setString("Start Game");
    startGameButtonText.setCharacterSize(15);
    startGameButtonText.setFillColor(sf::Color::White);
    startGameButtonText.setPosition(
        startGameButton.getPosition().x + 25,
        startGameButton.getPosition().y + 5);

}

GameEngine::~GameEngine()
{
    for (auto tower : mTowers) {
        delete tower;
    }
}

void GameEngine::run()
{
    while (mWindow.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void GameEngine::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            mWindow.close();
            break;

        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

                if (!mGameOver && !mLevelCompleted) {
                    if (mIsPaused) {
                        mSmallMenu.handleMouseClick(mousePos);
                    }
                    else {
                        if (mPlayerMenu.isMouseOverIcon(mousePos)) {
                            if (mPlayerMenu.isVisible()) {
                                mPlayerMenu.toggleSelect();
                            }
                            else {
                                mPlayerMenu.toggleSelect();
                                if (mTowerMenu.isVisible()) {
                                    mTowerMenu.hide();
                                    if (mSelectedTower) {
                                        mSelectedTower->setSelected(false);
                                        mSelectedTower = nullptr;
                                    }
                                }
                            }
                        }
                        else if (mTowerMenu.isVisible()) {
                            int zoneIndex = mTowerMenu.findUpgradeZone(mousePos);
                            if (zoneIndex != -1 && mSelectedTower) {
                                if (zoneIndex == 1) { // Check for sell zone
                                    mTowerMenu.handleSellClick(*mSelectedTower, mCrystals);
                                    mTowers.erase(std::remove(mTowers.begin(), mTowers.end(), mSelectedTower), mTowers.end()); // Remove the tower from the vector
                                    delete mSelectedTower; // Free the memory
                                    mSelectedTower = nullptr;
                                }
                                else {
                                    mTowerMenu.handleUpgradeClick(mousePos, *mSelectedTower, mCrystals);
                                }
                            }
                            else {
                                // Hide the menu if the click is outside
                                mTowerMenu.hide();
                                if (mSelectedTower) {
                                    mSelectedTower->setSelected(false);
                                    mSelectedTower = nullptr;
                                }
                            }
                        }
                        else if (mTowerSelectionMenu.isVisible()) {
                            if (mTowerSelectionMenu.isInsideMenu(mousePos)) {
                                // Place a new tower if clicking inside the selection menu
                                int selectedTowerType = mTowerSelectionMenu.getSelectedTowerType(mousePos);
                                if (selectedTowerType != -1) {
                                    Tower* newTower = nullptr;
                                    switch (selectedTowerType) {
                                    case 0: // Laser Tower
                                        newTower = new LaserTower(mMenuOpenPosition, mProjectiles);
                                        break;
                                    case 1: //Flame Turret
                                        newTower = new FlameTurret(mMenuOpenPosition, mProjectiles);
                                        break;
                                    }

                                    if (newTower) {
                                        if (mCrystals >= newTower->getCost()) {
                                            mTowers.push_back(newTower);
                                            mCrystals -= newTower->getCost();
                                            mTowerSelectionMenu.hide();
                                            mSelectedTowerType = -1;
                                        }
                                        else {
                                            std::cout << "Not enough crystals to buy tower" << std::endl;
                                            delete newTower;
                                        }
                                    }
                                }
                            }
                            else {
                                // Hide the tower selection menu if clicked outside
                                mTowerSelectionMenu.hide();
                            }
                        }
                        else {
                            if (!gameStarted) {
                                if (startGameButton.getGlobalBounds().contains(mousePos)) {
                                    gameStarted = true;
                                    continue;
                                }
                            }

                            // Handle tower selection, enemy selection or tower placement
                            bool towerClicked = false;
                            for (auto& tower : mTowers) {
                                if (tower->containsPoint(mousePos)) {
                                    if (mSelectedTower) {
                                        mSelectedTower->setSelected(false);
                                        mTowerMenu.hide();
                                    }
                                    mTowerSelectionMenu.hide();
                                    handleTowerClick(*tower, mousePos);
                                    towerClicked = true;
                                    break;
                                }
                            }
                            if (!towerClicked) {
                                handleNonTowerClick(mousePos);
                            }
                        }
                    }
                }
                else if (mGameOver) {
                    mGameOverMenu.handleMouseClick(mousePos);
                }
                else if (mLevelCompleted) {
                    mLevelCompleteMenu.handleMouseClick(mousePos);
                }
            }
            break;
        case sf::Event::KeyPressed:
            handleKeyPress(event.key.code);
            break;
        }
    }
}

void GameEngine::handleTowerClick(Tower& tower, const sf::Vector2f& mousePos) {
    // Deselect any previously selected tower
    if (mSelectedTower) {
        mSelectedTower->setSelected(false);
    }

    // Select the new tower
    tower.setSelected(true);
    mSelectedTower = &tower;

    // Check if the selected tower is within range of the player
    if (distance(mPlayer.getPosition(), mSelectedTower->getPosition()) <= mPlayer.getPlacementRange()) {
        mTowerMenu.show(tower);
        if (mPlayerMenu.isVisible())
            mPlayerMenu.toggleSelect();
    }
    else {
        std::cout << "Selected tower is out of range. Cannot open tower menu." << std::endl;
        mSelectedTower = nullptr;
    }
}

void GameEngine::handleNonTowerClick(const sf::Vector2f& worldPos) {
    if (distance(worldPos, mPlayer.getPosition()) <= mPlayer.getPlacementRange()) {
        if (mTowerSelectionMenu.isVisible()) {
            Tower* newTower = nullptr;
            switch (mSelectedTowerType) {
            case 0: // Laser Tower
                newTower = new LaserTower(worldPos, mProjectiles);
                break;
            case 1:
                newTower = new FlameTurret(worldPos, mProjectiles);
                break;
            }

            if (newTower) {
                if (mCrystals >= newTower->getCost()) {
                    std::cout << "Placing tower at: " << worldPos.x << ", " << worldPos.y << std::endl;
                    mTowers.push_back(newTower);
                    mCrystals -= newTower->getCost();
                    mTowerSelectionMenu.hide();
                }
                else {
                    std::cout << "Not enough crystals to place a tower!" << std::endl;
                    delete newTower;
                }
            }
        }
        else {
            mMenuOpenPosition = worldPos;
            mTowerSelectionMenu.show(worldPos);
        }
    }
    else {
        std::cout << "Out of range. Can't place tower." << std::endl;
    }
}

void GameEngine::handleKeyPress(sf::Keyboard::Key keyCode) {
    if (keyCode == sf::Keyboard::Tab) {
        if (mPlayerMenu.isVisible()) {
            mPlayerMenu.toggleSelect();
        }
        else {
            mPlayerMenu.toggleSelect();
            if (mTowerMenu.isVisible()) {
                mTowerMenu.hide();
                if (mSelectedTower) {
                    mSelectedTower->setSelected(false);
                    mSelectedTower = nullptr;
                }
            }
        }
    }
    else if (keyCode == sf::Keyboard::Escape) {
        if (mIsPaused) {
            mIsPaused = false;
            mSmallMenu.hide();
        }
        else {
            mIsPaused = true;
            if (mPlayerMenu.isVisible()) {
                mPlayerMenu.toggleSelect();
            }
            if (mTowerMenu.isVisible()) {
                mTowerMenu.hide();
                if (mSelectedTower) {
                    mSelectedTower->setSelected(false);
                    mSelectedTower = nullptr;
                }
            }
            mSmallMenu.show();
        }
    }
}

void GameEngine::update()
{
    if (!mGameOver && !mLevelCompleted) {
        if (mIsPaused) {
            sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
            mSmallMenu.updateHover(mousePos);
            return;
        }

        float dt = mClock.restart().asSeconds();

        mPlayer.update(dt, mEnemies);
        mPlayerMenu.updateHealthBar(mPlayer.getHealth());

        if (mPlayer.getHealth() <= 0) {
            mGameOver = true;
            return;
        }

        if (gameStarted) {

            // Update current wave
            mCurrentWave.update(dt, mEnemies);

            bool enemiesLeft = std::any_of(mEnemies.begin(), mEnemies.end(), [](const Enemy& enemy) {
                return !enemy.isDead(); // Check if there are any living enemies
                });

            if (mCurrentWave.isComplete() && !enemiesLeft)
            {
                if (mCurrentWaveNumber < 3) // Assuming 3 is the max number of waves
                {
                    std::cout << "Wave " << mCurrentWaveNumber << " complete. Transitioning to wave " << mCurrentWaveNumber + 1 << std::endl;

                    mCurrentWaveNumber++;
                    switch (mCurrentWaveNumber)
                    {
                    case 2:
                        mCurrentWave = Wave(mCurrentLevel, mCurrentWaveNumber, { 1 });
                        break;
                    case 3:
                        mCurrentWave = Wave(mCurrentLevel, mCurrentWaveNumber, { 0, 1 });
                        break;
                    }
                }
                else
                {
                    mLevelCompleted = true;
                }
            }

            // Update towers
            for (auto& tower : mTowers)
            {
                tower->update(dt, mEnemies);
            }

            // Update projectiles
            for (auto projectile = mProjectiles.begin(); projectile != mProjectiles.end();)
            {
                if (projectile->getTarget() == nullptr)
                {
                    projectile = mProjectiles.erase(projectile);
                    continue;
                }
                projectile->update(dt);

                if (projectile->hasHitTarget())
                {
                    projectile->getTarget()->takeDamage(projectile->getDamage());
                    projectile = mProjectiles.erase(projectile);
                }
                else
                {
                    ++projectile;
                }
            }

            // Update enemies
            for (auto enemy = mEnemies.begin(); enemy != mEnemies.end();)
            {
                enemy->update(dt, mPlayer);

                if (enemy->hasReachedEnd())
                {
                    mTowerHealth -= enemy->getDamageToTower();
                    enemy = mEnemies.erase(enemy);
                    if (mTowerHealth <= 0)
                    {
                        mGameOver = true;
                        return;
                    }
                    continue;
                }

                if (enemy->isDead())
                {
                    mCrystals += enemy->getReward();

                    enemy = mEnemies.erase(enemy);
                }
                else
                    ++enemy;
            }
        }

        mHealthBar.setSize(sf::Vector2f(200 * (mTowerHealth / 100.0f), 20));

        mCrystalText.setString("Crystals " + std::to_string(mCrystals));

        if (mTowerSelectionMenu.isVisible()) {
            if (distance(mPlayer.getPosition(), mMenuOpenPosition) > mPlayer.getPlacementRange()) {
                mTowerSelectionMenu.hide();
            }
            else {
                sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                mTowerSelectionMenu.updateHover(mousePos);
            }
        }

        // Close the tower menu if the selected tower is out of range
        if (mTowerMenu.isVisible() && mSelectedTower) {
            if (distance(mPlayer.getPosition(), mSelectedTower->getPosition()) > mPlayer.getPlacementRange()) {
                mTowerMenu.hide();
                mSelectedTower->setSelected(false);
                mSelectedTower = nullptr;
            }
            else {
                sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                mTowerMenu.update(mousePos, mCrystals, mCrystalText.getPosition(), mCrystalText.getLocalBounds());
            }
        }

        // Handle hover effects for buttons
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        if (!gameStarted)
            updateButtonHover(startGameButton, startGameButtonText, mousePos);
    }
    else if (mGameOver) {
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        mGameOverMenu.updateHover(mousePos);
    }
    else if (mLevelCompleted) {
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        mLevelCompleteMenu.updateHover(mousePos);
    }
}

void GameEngine::render()
{
    mWindow.clear();

    if (!mLevelCompleted) {

        mMap.render(mWindow);

        mPlayer.render(mWindow);
        for (const auto& tower : mTowers)
            tower->render(mWindow);
        for (auto& projectile : mProjectiles)
            projectile.render(mWindow);
        for (auto& enemy : mEnemies)
            enemy.render(mWindow);

        mWindow.draw(mHealthBarBackground);
        mWindow.draw(mHealthBar);
        mWindow.draw(mCrystalText);

        if (mTowerSelectionMenu.isVisible()) {
            sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
            mTowerSelectionMenu.updateHover(mousePos);
            mTowerSelectionMenu.render(mWindow);
        }

        if (mTowerMenu.isVisible() && mSelectedTower)
            mTowerMenu.render(mWindow);

        mPlayerMenu.render(mWindow, mPlayer);

        if (!gameStarted) {
            mWindow.draw(startGameButton);
            mWindow.draw(startGameButtonText);
        }

        if (mIsPaused)
            mSmallMenu.render(mWindow);

        if (mGameOver) {
            mGameOverMenu.render(mWindow);
        }
    }
    else {
        mLevelCompleteMenu.render(mWindow);
    }

    mWindow.display();
}

void GameEngine::updateButtonHover(sf::RectangleShape& button, sf::Text& buttonText, const sf::Vector2f& mousePos) {
    if (button.getGlobalBounds().contains(mousePos)) {
        button.setFillColor(sf::Color(120, 120, 120, 210));
    }
    else {
        button.setFillColor(sf::Color(0, 0, 0, 200));
    }
    buttonText.setFillColor(sf::Color::White);
}

bool GameEngine::isGameOver() const
{
    return mGameOver;
}

void GameEngine::init(int level)
{
    mPlayer.setHealth(100);
    mCurrentLevel = level;

    // Reset all variables and states to their initial values for the specified level
    mCrystals = 100;
    mTowers.clear();
    mEnemies.clear();
    mProjectiles.clear();
    mCurrentWaveNumber = 1;
    mTowerHealth = 100;
    gameStarted = false;
    mGameOver = false;
    mLevelCompleted = false;

    if (mIsPaused) {
        mIsPaused = false;
        mSmallMenu.hide();
    }

    mWindow.setView(mWindow.getDefaultView());

    mPlayer.setPosition(sf::Vector2f(100, 100));
}

bool GameEngine::isLevelCompleted(int level) const
{
    // Check if all waves are complete and there are no remaining enemies
    bool wavesComplete = !gameStarted || (mCurrentWaveNumber >= 3);
    bool noEnemiesLeft = std::none_of(mEnemies.begin(), mEnemies.end(), [](const Enemy& enemy) {
        return !enemy.isDead(); // There are no living enemies
        });

    return wavesComplete && noEnemiesLeft;
}
