#include "GameEngine.h"
#include "../../core/GameManager.h"
#include "../waves/PathsConfig.h"
#include "../../core/quests/QuestTypes.h"
#include <iostream>
#include <string.h>
#include <math.h>

static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

GameEngine::GameEngine(sf::RenderWindow& window, GameManager* gameManager)
    :
    mWindow(window),
    mSpawnTimer(0.0f),
    mTowerHealth(100),
    mCurrentLevel(1),
    mMap(mCurrentLevel),
    mSelectedTower(nullptr),
    mSelectedEnemy(nullptr),
    mSelectedTowerType(-1),
    mTowerMenu(),
    mTowerSelectionMenu(mAvailableTowers),
    mCrystals(100),
    mInitialCrystals(0),
    mPlayer(sf::Vector2f(100, 100), mProjectiles),
    mPlayerMenu(),
    gameStarted(false),
    mShowNotEnoughCrystalsText(false),
    mShowText1(false),
    mShowText2(false),
    mGameManager(gameManager),
    mSmallMenu(sf::Vector2f(window.getSize()), this, gameManager, mCurrentLevel, mAvailableTowers),
    mLevelCompleteMenu(sf::Vector2f(window.getSize()), this, nullptr, gameManager, mCurrentLevel, true),
    mGameOverMenu(sf::Vector2f(window.getSize()), this, gameManager, mCurrentLevel, mCrystals, mAvailableTowers) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font" << std::endl;

    mOutOfRangePlacement.setFont(mFont);
    mOutOfRangePlacement.setCharacterSize(14);
    mOutOfRangePlacement.setFillColor(sf::Color::White);
    mOutOfRangePlacement.setString("Out of range! Can't place tower.\nPress TAB to view the placement range");

    mOutOfRangeSelection.setFont(mFont);
    mOutOfRangeSelection.setCharacterSize(14);
    mOutOfRangeSelection.setFillColor(sf::Color::White);
    mOutOfRangeSelection.setString("Selected tower is out of range\nPress TAB to view the selection range!");

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

    //mTowerSelectionMenu = TowerSelectionMenu();
    //mTowerMenu = TowerMenu();
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
    startGameButtonText.setPosition(startGameButton.getPosition().x + 25,
                                    startGameButton.getPosition().y + 5);

    mNotEnoughCrystalsText.setFillColor(sf::Color::White);
    mNotEnoughCrystalsText.setFont(mFont);
    mNotEnoughCrystalsText.setCharacterSize(14);
    mNotEnoughCrystalsText.setString("Not enough crystals to buy tower!");

    mBorderUp.setPosition(sf::Vector2f(-10.f, -10.f));
    mBorderUp.setSize(sf::Vector2f(mWindow.getSize().x + 20.f, 10.f));
    mBorderUp.setFillColor(sf::Color::Transparent);

    mBorderDown.setPosition(sf::Vector2f(-10.f, mWindow.getSize().y / 1.f));
    mBorderDown.setSize(sf::Vector2f(mWindow.getSize().x + 20.f, 10.f));
    mBorderDown.setFillColor(sf::Color::Transparent);

    mBorderLeft.setPosition(sf::Vector2f(-10.f, -10.f));
    mBorderLeft.setSize(sf::Vector2f(10.f, mWindow.getSize().y + 20.f));
    mBorderLeft.setFillColor(sf::Color::Transparent);

    mBorderRight.setPosition(sf::Vector2f(mWindow.getSize().x / 1.f, -10.f));
    mBorderRight.setSize(sf::Vector2f(10.f, mWindow.getSize().y + 20.f));
    mBorderRight.setFillColor(sf::Color::Transparent);
}

GameEngine::~GameEngine(){
    for (auto tower : mTowers)
        delete tower;
}

void GameEngine::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                mGameManager->getGameEngine().saveGame();
                mWindow.close();
                break;

            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

                    if (!mGameOverMenu.isActive() && !mLevelCompleteMenu.isActive()) {
                        if (mSmallMenu.isActive()) {
                            mSmallMenu.handleMouseClick(mousePos);
                        } else {
                            if (mPlayerMenu.isMouseOverIcon(mousePos)) {
                                if (mPlayerMenu.isVisible()) {
                                    mPlayerMenu.toggleSelect();
                                } else {
                                    mPlayerMenu.toggleSelect();
                                    if (mTowerMenu.isVisible()) {
                                        mTowerMenu.hide();
                                        if (mSelectedTower) {
                                            mSelectedTower->setSelected(false);
                                            mSelectedTower = nullptr;
                                        }
                                    }
                                }
                            } else if (mTowerMenu.isVisible()) {
                                int zoneIndex = mTowerMenu.findUpgradeZone(mousePos);
                                if (zoneIndex != -1 && mSelectedTower) {
                                    if (zoneIndex == 1) { // Check for sell zone
                                        mTowerMenu.handleSellClick(*mSelectedTower, mCrystals);
                                        mTowers.erase(std::remove(mTowers.begin(), mTowers.end(), mSelectedTower), mTowers.end());
                                        delete mSelectedTower;
                                        mSelectedTower = nullptr;
                                    } else {
                                        mTowerMenu.handleUpgradeClick(mousePos, *mSelectedTower, mCrystals);
                                    }
                                } else {
                                    // Hide the menu if the click is outside
                                    mTowerMenu.hide();
                                    if (mSelectedTower) {
                                        mSelectedTower->setSelected(false);
                                        mSelectedTower = nullptr;
                                    }
                                }
                            } else if (mTowerSelectionMenu.isVisible()) {
                                if (mTowerSelectionMenu.isInsideMenu(mousePos)) {
                                    // Place a new tower if clicking inside the selection menu
                                    int selectedTowerType = mTowerSelectionMenu.getSelectedTowerType(mousePos);
                                    if (selectedTowerType != -1) {
                                        Tower* newTower = nullptr;
                                        sf::Vector2f position = mTowerSelectionMenu.getCenterPosition();
                                        switch (selectedTowerType) {
                                            case 0: // Laser Tower
                                                if (mAvailableTowers[0] == 1)
                                                    newTower = new LaserTower(position, mProjectiles);
                                                else if (mAvailableTowers[0] == 2)
                                                    newTower = new FlameTurret(position, mProjectiles);
                                                else if (mAvailableTowers[0] == 3)
                                                    newTower = new ThunderRod(position, mProjectiles, mEnemies);
                                                break;
                                            case 1:
                                                if (mAvailableTowers[1] == 1)
                                                    newTower = new LaserTower(position, mProjectiles);
                                                else if (mAvailableTowers[1] == 2)
                                                    newTower = new FlameTurret(position, mProjectiles);
                                                else if (mAvailableTowers[1] == 3)
                                                    newTower = new ThunderRod(position, mProjectiles, mEnemies);
                                                break;
                                            case 2:
                                                if (mAvailableTowers[2] == 1)
                                                    newTower = new LaserTower(position, mProjectiles);
                                                else if (mAvailableTowers[2] == 2)
                                                    newTower = new FlameTurret(position, mProjectiles);
                                                else if (mAvailableTowers[2] == 3)
                                                    newTower = new ThunderRod(position, mProjectiles, mEnemies);
                                                break;
                                        }

                                        if (newTower) {
                                            if (mCrystals >= newTower->getCost()) {
                                                mTowers.push_back(newTower);
                                                mCrystals -= newTower->getCost();
                                                mTowerSelectionMenu.hide();
                                                mSelectedTowerType = -1;
                                            } else {
                                                mNotEnoughCrystalsText.setPosition(sf::Vector2f(position.x - 104.35f, position.y - 81.f));
                                                mNotEnoughCrystalsText.setString("Not enough crystals to buy tower!");
                                                mNotEnoughCrystalsText.setFillColor(sf::Color::White);
                                                mNotEnoughCrystalsClock.restart();
                                                mShowNotEnoughCrystalsText = true;
                                                delete newTower;
                                            }
                                        }
                                    }
                                } else {
                                    // Hide the tower selection menu if clicked outside
                                    mTowerSelectionMenu.hide();
                                }
                            } else {
                                if (!gameStarted) {
                                    if (startGameButton.getGlobalBounds().contains(mousePos)) {
                                        gameStarted = true;
                                        mWaveManager->startNextWave();
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
                    } else if (mGameOverMenu.isActive()) {
                        mGameOverMenu.handleMouseClick(mousePos);
                    } else if (mLevelCompleteMenu.isActive()) {
                        mLevelCompleteMenu.handleMouseClick(mousePos);
                    }
                }
                break;
            case sf::Event::KeyPressed:
                handleKeyPress(event.key.code);
                break;

            default:
                break;
        }
    }
}

void GameEngine::handleTowerClick(Tower& tower, const sf::Vector2f& mousePos) {
    // Deselect any previously selected tower
    if (mSelectedTower)
        mSelectedTower->setSelected(false);

    // Select the new tower
    tower.setSelected(true);
    mSelectedTower = &tower;

    // Check if the selected tower is within range of the player
    if (distance(mPlayer.getPosition(), mSelectedTower->getPosition()) <= mPlayer.getPlacementRange()) {
        mTowerMenu.show(tower);
        if (mPlayerMenu.isVisible())
            mPlayerMenu.toggleSelect();
    } else {
        mOutOfRangeSelection.setFillColor(sf::Color::White);
        mShowText2 = true;
        mClockText2.restart();
        mOutOfRangeSelection.setPosition(sf::Vector2f(mSelectedTower->getPosition().x + 20.f,
            mSelectedTower->getPosition().y - 30.f));
        mSelectedTower = nullptr;
    }
}

void GameEngine::handleNonTowerClick(const sf::Vector2f& worldPos) {
    if (distance(worldPos, mPlayer.getPosition()) <= mPlayer.getPlacementRange()) {
        if (mTowerSelectionMenu.isVisible()) {
            Tower* newTower = nullptr;
            sf::Vector2f position = mTowerSelectionMenu.getCenterPosition();
            switch (mSelectedTowerType) {
                case 0:
                    if (mAvailableTowers[0] == 1)
                        newTower = new LaserTower(position, mProjectiles);
                    else if (mAvailableTowers[0] == 2)
                        newTower = new FlameTurret(position, mProjectiles);
                    else if (mAvailableTowers[0] == 3)
                        newTower = new ThunderRod(position, mProjectiles, mEnemies);
                    break;
                case 1:
                    if (mAvailableTowers[1] == 1)
                        newTower = new LaserTower(position, mProjectiles);
                    else if (mAvailableTowers[1] == 2)
                        newTower = new FlameTurret(position, mProjectiles);
                    else if (mAvailableTowers[1] == 3)
                        newTower = new ThunderRod(position, mProjectiles, mEnemies);
                    break;
                case 2:
                    if (mAvailableTowers[2] == 1)
                        newTower = new LaserTower(position, mProjectiles);
                    else if (mAvailableTowers[2] == 2)
                        newTower = new FlameTurret(position, mProjectiles);
                    else if (mAvailableTowers[2] == 3)
                        newTower = new ThunderRod(position, mProjectiles, mEnemies);
                    break;
            }

            if (newTower) {
                if (mCrystals >= newTower->getCost()) {
                    std::cout << "Placing tower at: " << worldPos.x << ", " << worldPos.y << std::endl;
                    mTowers.push_back(newTower);
                    mCrystals -= newTower->getCost();
                    mTowerSelectionMenu.hide();
                } else {
                    mNotEnoughCrystalsText.setPosition(sf::Vector2f(position.x - 104.35f, position.y - 81.f));
                    mNotEnoughCrystalsText.setString("Not enough crystals to buy tower!");
                    mNotEnoughCrystalsText.setFillColor(sf::Color::White);
                    mNotEnoughCrystalsClock.restart();
                    mShowNotEnoughCrystalsText = true;
                    delete newTower;
                }
            }
        } else {
            mMenuOpenPosition = worldPos;
            mTowerSelectionMenu.show(worldPos);
        }
    } else {
        mOutOfRangePlacement.setFillColor(sf::Color::White);
        mShowText1 = true;
        mClockText1.restart();
        mOutOfRangePlacement.setPosition(sf::Vector2f(worldPos.x - 20.f, worldPos.y - 20.f));
    }
}

void GameEngine::handleKeyPress(sf::Keyboard::Key keyCode) {
    if (keyCode == sf::Keyboard::Tab) {
        if (!mSmallMenu.isActive()) {
            if (mPlayerMenu.isVisible()) {
                mPlayerMenu.toggleSelect();
            } else {
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
    } else if (keyCode == sf::Keyboard::Escape) {
        if (mSmallMenu.isActive()) {
            mSmallMenu.setActive(false);
        } else {
            if (mPlayerMenu.isVisible()) {
                mPlayerMenu.toggleSelect();
            } else if (mTowerMenu.isVisible()) {
                mTowerMenu.hide();
                if (mSelectedTower) {
                    mSelectedTower->setSelected(false);
                    mSelectedTower = nullptr;
                }
            } else if (mTowerSelectionMenu.isVisible()) {
                mTowerSelectionMenu.hide();
            } else {
                mSmallMenu.setActive(true);
            }
        }
    }
}

void GameEngine::update(float mdt) {
    float dt = mClock.restart().asSeconds();
    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
    mSmallMenu.update(dt);
    mSmallMenu.updateHover(mousePos);

    if (!mGameOverMenu.isActive() && !mLevelCompleteMenu.isActive() && !mSmallMenu.isActive()) {
        mPlayer.update(dt, mEnemies, mBorderUp, mBorderDown, mBorderLeft, mBorderRight);
        mPlayerMenu.updateHealthBar(mPlayer.getHealth());

        if (mPlayer.getHealth() <= 0) {
            mGameOverMenu.setActive(true);
            return;
        }

        if (gameStarted) {
            // Update current wave
            mWaveManager->update(dt, mEnemies);

            bool enemiesLeft = std::any_of(mEnemies.begin(), mEnemies.end(), [](const Enemy& enemy) {
                return !enemy.isDead(); // Check if there are any living enemies
                });

            if (mWaveManager->isWaveComplete() && !enemiesLeft) {
                if (!mWaveManager->isLevelComplete())
                    mWaveManager->startNextWave();
                else
                    mLevelCompleteMenu.setActive(true);
            }

            // Update towers
            for (auto& tower : mTowers)
                tower->update(dt, mEnemies);

            // Update projectiles
            for (auto projectile = mProjectiles.begin(); projectile != mProjectiles.end();) {
                projectile->update(dt);

                if (projectile->shouldRemove())
                    projectile = mProjectiles.erase(projectile);
                else
                    ++projectile;
            }

            // Update enemies
            for (auto enemy = mEnemies.begin(); enemy != mEnemies.end();) {
                enemy->update(dt, mPlayer);

                if (enemy->hasReachedEnd()) {
                    mTowerHealth -= enemy->getDamageToTower();
                    enemy = mEnemies.erase(enemy);
                    if (mTowerHealth <= 0) {
                        mGameOverMenu.setActive(true);
                        return;
                    }
                    continue;
                }

                if (enemy->isDead()) {
                    mCrystals += enemy->getReward();

                    mEnemiesKilledInRound++;
                    mCrystalsEarnedInRound += enemy->getReward();

                    mGameManager->dispatchQuestEvent(GameEvent{ObjectiveType::kill_enemy, enemy->getId(), 1});

                    enemy = mEnemies.erase(enemy);
                } else {
                    ++enemy;
                }
            }
        }

        mHealthBar.setSize(sf::Vector2f(200 * (mTowerHealth / 100.0f), 20));

        mCrystalText.setString("Crystals " + std::to_string(mCrystals));

        if (mTowerSelectionMenu.isVisible()) {
            if (distance(mPlayer.getPosition(), mMenuOpenPosition) > mPlayer.getPlacementRange()) {
                mTowerSelectionMenu.hide();
            } else {
                mTowerSelectionMenu.updateHover(mousePos, mCrystals);
            }
        }

        // Close the tower menu if the selected tower is out of range
        if (mTowerMenu.isVisible() && mSelectedTower) {
            if (distance(mPlayer.getPosition(), mSelectedTower->getPosition()) > mPlayer.getPlacementRange()) {
                mTowerMenu.hide();
                mSelectedTower->setSelected(false);
                mSelectedTower = nullptr;
            } else {
                mTowerMenu.update(mousePos, mCrystals, mCrystalText.getPosition(), mCrystalText.getLocalBounds());
            }
        }

        if (mShowText1) {
            float elapsedTime1 = mClockText1.getElapsedTime().asSeconds();
            if (elapsedTime1 > 1.8f) {
                mShowText1 = false;
            } else {
                int alpha = static_cast<int>(255 * (1.0f - (elapsedTime1 / 1.8f)));
                mOutOfRangePlacement.setFillColor(sf::Color(255, 255, 255, alpha));
            }
        }

        if (mShowText2) {
            float elapsedTime2 = mClockText2.getElapsedTime().asSeconds();
            if (elapsedTime2 > 1.8f) {
                mShowText2 = false;
            } else {
                int alpha = static_cast<int>(255 * (1.0f - (elapsedTime2 / 1.8f)));
                mOutOfRangeSelection.setFillColor(sf::Color(255, 255, 255, alpha));
            }
        }

        // Handle hover effects for buttons
        if (!gameStarted)
            updateButtonHover(startGameButton, startGameButtonText, mousePos);
    } else if (mGameOverMenu.isActive()) {
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        mGameOverMenu.updateHover(mousePos);
        mGameOverMenu.update(dt);
    } else if (mLevelCompleteMenu.isActive()) {
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        mLevelCompleteMenu.updateHover(mousePos);
    }
}

void GameEngine::render() {
    mMap.render(mWindow);

    mPlayer.render(mWindow);

    mWindow.draw(mBorderDown);
    mWindow.draw(mBorderUp);
    mWindow.draw(mBorderLeft);
    mWindow.draw(mBorderRight);

    for (const auto& tower : mTowers)
        tower->render(mWindow);
    for (auto& projectile : mProjectiles)
        projectile.render(mWindow);
    for (auto& enemy : mEnemies)
        enemy.render(mWindow);

    mWindow.draw(mHealthBarBackground);
    mWindow.draw(mHealthBar);
    mWindow.draw(mCrystalText);

    if (mTowerSelectionMenu.isVisible())
        mTowerSelectionMenu.render(mWindow);

    if (mTowerMenu.isVisible() && mSelectedTower)
        mTowerMenu.render(mWindow);

    mPlayerMenu.render(mWindow, mPlayer);

    if (!gameStarted) {
        mWindow.draw(startGameButton);
        mWindow.draw(startGameButtonText);
    }

    if (mShowText2)
        mWindow.draw(mOutOfRangeSelection);

    if (mShowText1)
        mWindow.draw(mOutOfRangePlacement);

    // If the error text is visible gradually make it dissapear and render it
    if (mShowNotEnoughCrystalsText) {
        float elapsedTime = mNotEnoughCrystalsClock.getElapsedTime().asSeconds();
        if (elapsedTime > 1.8f) {
            mShowNotEnoughCrystalsText = false;
            mNotEnoughCrystalsText.setString("");
        }
        else {
            int alpha = static_cast<int>(255 * (1.0f - (elapsedTime / 1.8f)));
            mNotEnoughCrystalsText.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        mWindow.draw(mNotEnoughCrystalsText);
    }

    mLevelCompleteMenu.render(mWindow);
    mGameOverMenu.render(mWindow);
    mSmallMenu.render(mWindow);
}
void GameEngine::updateButtonHover(sf::RectangleShape& button, sf::Text& buttonText, const sf::Vector2f& mousePos) {
    if (button.getGlobalBounds().contains(mousePos))
        button.setFillColor(sf::Color(120, 120, 120, 210));
    else
        button.setFillColor(sf::Color(0, 0, 0, 200));

    buttonText.setFillColor(sf::Color::White);
}

bool GameEngine::isGameOver() const {
    return mGameOverMenu.isActive();
}

void GameEngine::init(int level, int crystals, const std::vector<int>& availableTowers) {
    mPlayer.setHealth(100);
    mCurrentLevel = level;

    // Reset all variables and states to their initial values for the specified level
    mCrystals = crystals;
    mInitialCrystals = crystals;
    mTowers.clear();
    mEnemies.clear();
    mProjectiles.clear();

    mTowerHealth = 100;
    gameStarted = false;

    mEnemiesKilledInRound = 0;
    mCrystalsEarnedInRound = 0;

    mAvailableTowers = availableTowers;
    mSmallMenu.setActive(false);
    mGameOverMenu.setActive(false);
    mLevelCompleteMenu.setActive(false);

    mWindow.setView(mWindow.getDefaultView());

    mPlayer.setPosition(sf::Vector2f(100, 100));
    mPlayer.setAnimation(4);
    mWaveDatabase.loadFromFile("assets/waves/waves.json");
    mWaveManager = std::make_unique<WaveManager>(mWaveDatabase, level);
}

int GameEngine::getCrystals() const {
    return mCrystals;
}

int GameEngine::getInitialCrystals() {
    return mInitialCrystals;
}

int GameEngine::getCurrentWave() const {
    if (mWaveManager)
        return mWaveManager->getCurrentWaveIndex() + 1;
    return 0;
}

int GameEngine::getMaxWaves() const {
    if (mWaveManager)
        return mWaveManager->getLevelConfig()->waves.size();
    return 0;
}
