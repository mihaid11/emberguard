#include "RPGEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <math.h>
#include "../../GameManager.h"

// TODO : make the wrapText method work in order to show the NPC dialogue lines
std::vector<sf::String> wrapText(const sf::String& text, const sf::Font& font, unsigned int characterSize, float boxWidth) {
    std::vector<sf::String> lines;
    sf::String currentLine;
    std::istringstream wordStream(text.toAnsiString());  // Convert to ANSI string for stream processing
    std::string word;

    while (wordStream >> word) {
        // Measure the size of the current line with the new word appended
        sf::Text tempText(currentLine + (currentLine.isEmpty() ? "" : " ") + word, font, characterSize);
        float lineWidth = tempText.getLocalBounds().width;

        if (lineWidth > boxWidth) {
            // If the current line with the new word exceeds the box width, store the current line and start a new one
            lines.push_back(currentLine);
            currentLine = word;
        }
        else {
            if (!currentLine.isEmpty()) {
                currentLine += " ";
            }
            currentLine += word;
        }
    }

    // Add the last line to the result
    if (!currentLine.isEmpty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

static bool intersects(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
    return rect1.intersects(rect2);
}

RPGEngine::RPGEngine(sf::RenderWindow& window, GameManager* gameManager)
    : mWindow(window),
    mMap(),
    mTimeSystem(0.36f),
    mCharacter(sf::Vector2f(400.f, 300.f), mMap),
    mView(sf::Vector2f(400.f, 300.f), sf::Vector2f(740.f, 420.f)),
    mFixedCamera(sf::Vector2f(0.f, 0.f), sf::Vector2f(740.f, 420.f)),
    mCameraFixedPosition(sf::Vector2f(0.f, 0.f)),
    mShowDialogue(false),
    mGameManager(gameManager),
    mSaveSystem(),
    mSaveNumber(0),
    mMiraStanton(sf::Vector2f(1000.0f, 900.0f), "mira_stanton"),
    mElliotMarlowe(sf::Vector2f(100.0f, 0.0f), "elliot_marlowe"),
    mGarrickStone(sf::Vector2f(-100.0f, 0.0f), "garrick_stone",gameManager),
    mVincentHale(sf::Vector2f(600.0f, 400.0f), "vincent_hale"),
    mSeraphinaLumeris(sf::Vector2f(-20.0f, 980.0f), "seraphina_lumeris"),
    mNPCManager(mStoryManager, mDialogueDatabase),
    mChestInventory(2, 2),
    mCurrentInteractingNPC(nullptr),
    mSkillTree(),
    mMenu(window, mSkillTree, mInventory, mCharacter.getPosition(), mDroppedItems, *this, gameManager),
    mHotbar(mInventory, sf::Vector2f(mWindow.getSize().x / 2 - 45.f * 3 / 2, mWindow.getSize().y - 45.f - 4.f),
        mInventory.getSlotCount() / 2, sf::Vector2f(45.f, 45.f)),
    mShowMenu(false),
    mShowStartMenu(false),
    mShowShopMenu(false),
    mShowBankMenu(false),
    mShowAnalyzeMenu(false),
    mShowInteract(false),
    mIsInsideAStructure(false),
    mCrystals(100),
    mStorageCapacity(500),
    mCurrentLevel(1),
    mChapter(1),
    mSelectedChoice(0),
    //mZoneManager(),
    mStartTowerDefenseMenu(window, mAvailableTowers, this, gameManager, mCurrentLevel, mCrystals),
    mBankMenu(window, mCrystals, mStorageCapacity, mTimeSystem),
    mShopMenu(window, mInventory, 5, mCrystals),
    mAnalyzeMenu(window, mInventory, mTimeSystem, mAvailableTowers, sf::Vector2f(70.0f, 70.0f), mCrystals),
    mChestMenu(window, mInventory, mChestInventory, sf::Vector2f(70.f, 70.f)),
    mTransitionSystem(sf::Vector2f(window.getSize().x, window.getSize().y)) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font from file" << std::endl;

    mDialogueText.setFont(mFont);
    mDialogueText.setCharacterSize(10);
    mDialogueText.setFillColor(sf::Color::White);
    mDialogueText.setPosition(400.f, 500.f);

    mInteractCircle.setRadius(7.5f);
    mInteractCircle.setFillColor(sf::Color(30, 30, 30, 200));
    mInteractCircle.setOutlineColor(sf::Color::White);
    mInteractCircle.setOutlineThickness(0.7f);

    mInteractText.setFont(mFont);
    mInteractText.setCharacterSize(7);
    mInteractText.setFillColor(sf::Color::White);
    mInteractText.setString("E");

    mDialogueInteractCircle.setRadius(13.f);
    mDialogueInteractCircle.setFillColor(sf::Color(30, 30, 30, 200));
    mDialogueInteractCircle.setOutlineColor(sf::Color::White);
    mDialogueInteractCircle.setOutlineThickness(0.7f);

    mDialogueInteractText.setFont(mFont);
    mDialogueInteractText.setCharacterSize(10);
    mDialogueInteractText.setFillColor(sf::Color::White);
    mDialogueInteractText.setString("E");

    // Map barriers
    mMap.addEntity<Barrier>(sf::Vector2f(-10.f, -10.f), sf::Vector2f(mMap.getSize().x + 20.f, 10.f));
    mMap.addEntity<Barrier>(sf::Vector2f(-10.f, -10.f), sf::Vector2f(10.f, mMap.getSize().y + 20.f));
    mMap.addEntity<Barrier>(sf::Vector2f(mMap.getSize().x, -10.f), sf::Vector2f(10.f, mMap.getSize().y + 20.f));
    mMap.addEntity<Barrier>(sf::Vector2f(-10.f, mMap.getSize().y), sf::Vector2f(mMap.getSize().y + 20.f, 10.f));

    // MCHouseInt barriers
    // Left
    mMap.addEntity<Barrier>(sf::Vector2f(-991.75f, -935.f), sf::Vector2f(30.f, 420.f));
    // Right
    mMap.addEntity<Barrier>(sf::Vector2f(-534.5f, -920.f), sf::Vector2f(30.f, 260.f));
    // Bottom Left
    mMap.addEntity<Barrier>(sf::Vector2f(-1007.f, -586.3f), sf::Vector2f(105.f, 25.f));
    // Bottom Right
    mMap.addEntity<Barrier>(sf::Vector2f(-858.f, -586.3f), sf::Vector2f(107.f, 25.f));
    // Middle Right
    mMap.addEntity<Barrier>(sf::Vector2f(-791.f, -696.5f), sf::Vector2f(325.f, 25.f));
    // Middle Up
    mMap.addEntity<Barrier>(sf::Vector2f(-794.f, -685.f), sf::Vector2f(30.f, 130.f));
    // Bottom Middle
    mMap.addEntity<Barrier>(sf::Vector2f(-903.f, -561.f), sf::Vector2f(60.f, 20.f));

    // MCHouseInt bed
    mMap.addEntity<Bed>(sf::Vector2f(-639.f, -810.f), 1.8f, "assets/sprites/buildings/bed.png",
                        sf::Vector2f(-634.f, -815.f), sf::Vector2f(75.2f, 1.f),
                        sf::Vector2f(-640.f, -816.f), sf::Vector2f(90.f, 41.f),
                        mTimeSystem, mTransitionSystem, gameManager);
    // TODO: Add chest sprite
    mMap.addEntity<Chest>(sf::Vector2f(-750.f, -810.f), 1.8f, "assets/sprites/buildings/bed.png",
                          sf::Vector2f(-745.f, -815.f), sf::Vector2f(75.2f, 1.f),
                          sf::Vector2f(-751.f, -816.f), sf::Vector2f(90.f, 41.f), mShowChestMenu);
    mMap.addEntity<MCHouse>(sf::Vector2f(300.f, 300.f), "assets/sprites/buildings/mcHouseExt.png",
                            sf::Vector2f(313.f, 481.f), sf::Vector2f(143.f, 30.f), 1, sf::Vector2f(340.f, 505.f),
                            sf::Vector2f(60.f, 30.f), mCharacter, mIsInsideAStructure, mCameraFixedPosition);
    mMap.addEntity<MCHouseInt>(sf::Vector2f(-990.f, -954.f), "assets/sprites/buildings/mcHouseInt.png",
                               sf::Vector2f(-980.f, -898.f), sf::Vector2f(580.f, 40.f), 1, sf::Vector2f(-903.f, -565.f),
                               sf::Vector2f(48.f, 18.f), mCharacter, mIsInsideAStructure, mCameraFixedPosition);

    //mZoneManager.loadTileset("assets/sprites/tiles/Tileset.png");

    //Add NPCs dialogues
    mDialogueDatabase.loadDialogueFromFile("dialogues/mira_stanton.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/seraphina_lumeris.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/garrick_stone.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/vincent_hale.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/elliot_marlowe.json");

    // Add NPCs
    mNPCManager.addNPC(std::make_unique<GarrickStone>(sf::Vector2f(650.0f, 400.0f), "garrick_stone", gameManager));
    mNPCManager.addNPC(std::make_unique<MiraStanton>(sf::Vector2f(1000.0f, 900.0f), "mira_stanton"));
    mNPCManager.addNPC(std::make_unique<ElliotMarlowe>(sf::Vector2f(100.0f, 0.0f), "elliot_marlowe"));
    mNPCManager.addNPC(std::make_unique<VincentHale>(sf::Vector2f(-100.0f, 0.0f), "vincent_hale"));
    mNPCManager.addNPC(std::make_unique<SeraphinaLumeris>(sf::Vector2f(-20.0f, 980.0f), "seraphina_lumeris"));
    mStoryManager.bindNPCManager(&mNPCManager);
    mStoryManager.setChapter(1);

    mAvailableTowers.push_back(2);
    mAvailableTowers.push_back(1);
    mAvailableTowers.push_back(3);

    // Testing
    std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
    mInventory.addItem(std::move(woodItem), 1);

    std::unique_ptr<TowerBlueprint> towerB = std::make_unique<TowerBlueprint>();
    mInventory.addItem(std::move(towerB), 5);

    std::unique_ptr<TowerBlueprintEpic> towerBE = std::make_unique<TowerBlueprintEpic>();
    mInventory.addItem(std::move(towerBE), 2);

    if(mSaveNumber != 0) {
        if (saveExists(mSaveNumber)) {
            loadGame();
        } else {
            newGame();
            mCharacter.setPosition(sf::Vector2f(340.f, 560.f));
            mCharacter.setAnimation(4);
        }
    }
}

void RPGEngine::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            saveGame();
            mWindow.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (mTransitionSystem.isTransitioning())
                return;

            if (mShowDialogue && mNPCManager.currentNPCHasChoices()) {
                auto choices = mNPCManager.getCurrentNPCChoices();

                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                    mSelectedChoice = (mSelectedChoice - 1 + choices.size()) % choices.size();
                    return;
                } else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                    mSelectedChoice = (mSelectedChoice + 1) % choices.size();
                    return;
                } else if (event.key.code == sf::Keyboard::E || event.key.code == sf::Keyboard::Return) {
                    mNPCManager.selectChoiceForCurrentNPC(mSelectedChoice, mShowDialogue, mDialogueText);
                    mSelectedChoice = 0;
                    return;
                }
            }

            if (event.key.code == sf::Keyboard::E) {
                if (!mShowDialogue) {
                    mNPCManager.handleInteraction(mCharacter, mShowDialogue, mDialogueText);
                    if (mShowDialogue) {
                        mCurrentInteractingNPC = mNPCManager.getCurrentNPC();
                    } else {
                        for (auto& entity : mMap.getEntities()) {
                            if (entity->isInteractable())
                                if (entity->getInteractBounds().intersects(mCharacter.getBounds())) {
                                    entity->interact();
                                    break;
                                }
                        }
                    }
                } else if (mCurrentInteractingNPC && mShowDialogue) {
                    if (!mNPCManager.currentNPCHasChoices())
                        mNPCManager.interactWithCurrentNPC(mShowDialogue, mDialogueText);
                } else {
                    mShowDialogue = false;
                    if (mCurrentInteractingNPC) {
                        mCurrentInteractingNPC->resumeMovement();
                        mCurrentInteractingNPC->resetDialogue();
                        mCurrentInteractingNPC = nullptr;
                    }
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                if (mShowDialogue) {
                    mShowDialogue = false;
                    mSelectedChoice = 0;
                    if (mCurrentInteractingNPC) {
                        mCurrentInteractingNPC->resumeMovement();
                        mCurrentInteractingNPC->resetDialogue();
                        mCurrentInteractingNPC = nullptr;
                    }
                } else if (mShowStartMenu) {
                    mShowStartMenu = false;
                } else if (mShowBankMenu) {
                    mShowBankMenu = false;
                    mBankMenu.restart();
                } else if (mShowShopMenu) {
                    mShowShopMenu = false;
                } else if (mShowAnalyzeMenu) {
                    mShowAnalyzeMenu = false;
                } else if (mShowChestMenu) {
                    mShowChestMenu = false;
                } else {
                    mShowMenu = !mShowMenu;
                    if (!mShowMenu)
                        mMenu.restart();
                }
            } else if (event.key.code == sf::Keyboard::O) {
                sf::String original = mDialogueText.getString();
                std::string text1 = original.toAnsiString();
                std::cout << text1 << std::endl;
                std::vector<sf::String> lines = wrapText(text1, mFont, 10, 425.0f);
                for (const auto& line : lines) {
                    std::cout << line.toAnsiString() << std::endl;
                }
            } else if (event.key.code == sf::Keyboard::L) {
                mShowStartMenu = !mShowStartMenu;

                if (mShowStartMenu) {
                    saveGame();
                    mShowMenu = false;
                    mShowBankMenu = false;
                    mShowShopMenu = false;
                    mShowAnalyzeMenu = false;
                    mShowChestMenu = false;
                    if (mShowDialogue) {
                        mShowDialogue = false;
                        if (mCurrentInteractingNPC) {
                            mCurrentInteractingNPC->resetDialogue();
                            mCurrentInteractingNPC->resumeMovement();
                            mCurrentInteractingNPC = nullptr;
                        }
                    }
                }
            } else if (event.key.code == sf::Keyboard::B) {
                mShowBankMenu = !mShowBankMenu;

                if (mShowBankMenu) {
                    mShowMenu = false;
                    mShowStartMenu = false;
                    mShowShopMenu = false;
                    mShowAnalyzeMenu = false;
                    mShowChestMenu = false;
                    if (mShowDialogue) {
                        mShowDialogue = false;
                        if (mCurrentInteractingNPC) {
                            mCurrentInteractingNPC->resetDialogue();
                            mCurrentInteractingNPC->resumeMovement();
                            mCurrentInteractingNPC = nullptr;
                        }
                    }
                } else
                    mBankMenu.restart();
            } else if (event.key.code == sf::Keyboard::K) {
                if (!mShowShopMenu) {
                    mShowShopMenu = true;
                    if (mShowDialogue) {
                        mShowDialogue = false;
                        if (mCurrentInteractingNPC) {
                            mCurrentInteractingNPC->resetDialogue();
                            mCurrentInteractingNPC->resumeMovement();
                            mCurrentInteractingNPC = nullptr;
                        }
                    }

                    mShowMenu = false;
                    mShowBankMenu = false;
                    mShowStartMenu = false;
                    mShowAnalyzeMenu = false;
                    mShowChestMenu = false;
                } else
                    mShowShopMenu = false;
            } else if (event.key.code == sf::Keyboard::Q) {
                if (mShowMenu && mMenu.getMenuType() == "Inventory") {
                    int slotIndex = mMenu.getInventoryMenu().getHoveredSlot();
                    if (slotIndex != -1) {
                        if (mInventory.getItemAt(slotIndex)) {
                            const Item* item = mInventory.getItemAt(slotIndex);
                            sf::Vector2f droppedItemPos;
                            if (mCharacter.getAnimation() == 1)
                                droppedItemPos = {-35.5f, 2.f};
                            else if (mCharacter.getAnimation() == 2)
                                droppedItemPos = {13.f, 2.f};
                            else if (mCharacter.getAnimation() == 3)
                                droppedItemPos = {-11.5f, -mCharacter.getHeight() / 1.1f};
                            else if (mCharacter.getAnimation() == 4)
                                droppedItemPos = {-11.5f, mCharacter.getHeight() / 2.f};
                            const DroppedItem dropItem = DroppedItem(item,
                                mCharacter.getCenterPosition() + droppedItemPos,
                                mInventory.getItemQuantityAt(slotIndex));

                            mInventory.removeItemAt(slotIndex);
                            mDroppedItems.push_back(dropItem);
                        }
                    }
                }

                if (!mShowMenu && !mShowBankMenu && !mShowStartMenu && !mShowDialogue && !mShowAnalyzeMenu && !mShowChestMenu) {
                    int slotIndex = mHotbar.getHoveredSlot();
                    if (slotIndex != -1) {
                        if (mInventory.getItemAt(slotIndex)) {
                            const Item* item = mInventory.getItemAt(slotIndex);
                            sf::Vector2f droppedItemPos;
                            if (mCharacter.getAnimation() == 1)
                                droppedItemPos = {-35.5f, 2.f};
                            else if (mCharacter.getAnimation() == 2)
                                droppedItemPos = {13.f, 2.f};
                            else if (mCharacter.getAnimation() == 3)
                                droppedItemPos = {-11.5f, -mCharacter.getHeight() / 1.1f};
                            else if (mCharacter.getAnimation() == 4)
                                droppedItemPos = {-11.5f, mCharacter.getHeight() / 2.f};
                            const DroppedItem dropItem = DroppedItem(item,
                                mCharacter.getCenterPosition() + droppedItemPos,
                                mInventory.getItemQuantityAt(slotIndex));

                            mInventory.removeItemAt(slotIndex);
                            mDroppedItems.push_back(dropItem);
                        }
                    }
                }
            } else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Num3) {
                if (!mShowBankMenu && !mShowDialogue && !mShowMenu && !mShowStartMenu && !mShowAnalyzeMenu && !mShowChestMenu) {
                    int slot = -1;
                    if (event.key.code == sf::Keyboard::Num1)
                        slot = 0;
                    else if (event.key.code == sf::Keyboard::Num2)
                        slot = 1;
                    else if (event.key.code == sf::Keyboard::Num3)
                        slot = 2;
                    mHotbar.setHoveredSlot(slot);
                }
            } else if (event.key.code == sf::Keyboard::M) {
                mShowAnalyzeMenu = !mShowAnalyzeMenu;
                if (mShowAnalyzeMenu) {
                    mShowMenu = false;
                    mShowStartMenu = false;
                    mShowShopMenu = false;
                    mShowBankMenu = false;
                    mShowChestMenu = false;
                    if (mShowDialogue) {
                        mShowDialogue = false;
                        if (mCurrentInteractingNPC) {
                            mCurrentInteractingNPC->resetDialogue();
                            mCurrentInteractingNPC->resumeMovement();
                            mCurrentInteractingNPC = nullptr;
                        }
                    }
                }
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                if (mShowDialogue && mNPCManager.currentNPCHasChoices()) {
                    for (size_t i = 0; i < mChoiceBoxes.size(); ++i) {
                        if (mChoiceBoxes[i].getGlobalBounds().contains(mousePos)) {
                            mNPCManager.selectChoiceForCurrentNPC(i, mShowDialogue, mDialogueText);
                            mSelectedChoice = 0;
                            return;
                        }
                    }
                }

                if (mShowMenu)
                    mMenu.handleMouseClick(mousePos);
                if (mShowStartMenu)
                    mStartTowerDefenseMenu.handleMouseClick(mousePos);
                if (mShowBankMenu)
                    mBankMenu.handleMouseClick(mousePos);
                if (mShowShopMenu)
                    mShopMenu.handleMouseClick(mousePos);
                if (mShowAnalyzeMenu)
                    mAnalyzeMenu.handleMouseClick(mousePos);
                if (mShowChestMenu)
                    mChestMenu.handleMouseClick(mousePos);

                if(!mShowMenu && !mShowStartMenu && !mShowBankMenu && !mShowShopMenu && !mShowAnalyzeMenu && !mShowChestMenu && !mShowDialogue) {
                    int slot = mHotbar.contains(mousePos);
                    mHotbar.setHoveredSlot(slot);
                }
            }
        }
    }
}

void RPGEngine::update() {
    float dt = mClock.restart().asSeconds();

    if (!mShowMenu) {
        if (!mShowStartMenu) {
            if (!mShowBankMenu) {
                if (!mShowShopMenu) {
                    if (!mShowAnalyzeMenu) {
                        if (!mShowChestMenu) {
                            sf::Vector2f previousPosition = mCharacter.getPosition();

                            if (!mShowDialogue) {
                                mHotbar.update();
                            }

                            mTransitionSystem.update(dt);

                            mTimeSystem.update(dt);

                            if (mTransitionSystem.isTransitioning())
                                return;

                            mCharacter.update(dt, mShowDialogue);
                            //mZoneManager.update(mCharacter.getPosition());

                            if (mMap.checkCollision(mCharacter.getBounds()))
                                mCharacter.setPosition(previousPosition);

                            for (auto& npc : mNPCManager.getNPCs()) {
                                if (npc->getBounds().intersects(mCharacter.getBounds())) {
                                    sf::Vector2f npcPos = npc->getPosition();
                                    sf::Vector2f direction = previousPosition - npcPos;
                                    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                                    if (distance < 10.0f) {  // Push the player away from the npc with a threshold
                                        direction /= distance;
                                        previousPosition += direction * 5.0f * dt;
                                        mCharacter.setPosition(previousPosition);
                                    }
                                }

                                if (npc->isPlayerClose(mCharacter.getPosition())) {
                                    if (mShowDialogue)
                                        npc->setInteract(false);
                                    else
                                        npc->setInteract(true);
                                }
                                else
                                    npc->setInteract(false);
                            }

                            mNPCManager.update(dt);
                            mView.setCenter(mCharacter.getCenterPosition());
                            mFixedCamera.setCenter(mCameraFixedPosition);

                            if (!mNPCManager.playerClose(mCharacter.getPosition())) {
                                mShowDialogue = false;
                            }

                            for (auto it = mDroppedItems.begin(); it != mDroppedItems.end();) {
                                float distance = std::sqrt(
                                    std::pow(mCharacter.getPosition().x - it->getPosition().x, 2.f) +
                                    std::pow(mCharacter.getPosition().y - it->getPosition().y, 2.f)
                                );

                                if (it->getPickUpCap()) {
                                    if (distance < 35.f) {
                                        const Item* item = it->getItem();
                                        if (item->getId() == 1)
                                            mInventory.addItem(std::make_unique<Wood>(), it->getQuantity());
                                        else if (item->getId() == 2)
                                            mInventory.addItem(std::make_unique<TowerBlueprint>(), it->getQuantity());
                                        else if (item->getId() == 3)
                                            mInventory.addItem(std::make_unique<TowerBlueprintRare>(), it->getQuantity());
                                        else if (item->getId() == 4)
                                            mInventory.addItem(std::make_unique<TowerBlueprintEpic>(), it->getQuantity());
                                        else if(item->getId() == 5)
                                            mInventory.addItem(std::make_unique<TowerBlueprintMythic>(), it->getQuantity());

                                        it = mDroppedItems.erase(it);
                                        continue;
                                    }
                                } else {
                                    if (distance > 35.f)
                                        it->setPickUpCap(true);
                                }
                                ++it;
                            }
                            mMap.update();

                            mShowInteract = false;

                            for (auto& entity : mMap.getEntities()) {
                                if (!entity->isInteractable())
                                    continue;

                                if (entity->getInteractBounds().intersects(mCharacter.getBounds())) {
                                    mShowInteract = true;

                                    mInteractPos = entity->getInteractPosition();

                                    mInteractCircle.setPosition({mInteractPos.x +
                                        entity->getInteractBounds().width + 1.5f, mInteractPos.y});

                                    mInteractText.setPosition({ mInteractCircle.getPosition().x + 5.9f,
                                        mInteractCircle.getPosition().y + 2.8f});

                                    break;
                                }
                            }

                            if (!mShowInteract) {
                                mInteractPos = { 0.f, 0.f };
                                mInteractCircle.setPosition(mInteractPos);
                                mInteractText.setPosition(mInteractPos);
                            }

                            mAnalyzeMenu.update();
                        } else {
                            sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                            mChestMenu.updateHover(mousePos);
                        }
                    } else {
                        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                        mAnalyzeMenu.updateHover(mousePos);
                    }
                } else {
                    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                    mShopMenu.updateHover(mousePos);
                }
            } else {
                sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
                mBankMenu.updateHover(mousePos);
                mBankMenu.update();
            }
        } else {
            sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
            mStartTowerDefenseMenu.updateHover(mousePos);
        }

    } else {
        sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        mMenu.updateHover(mousePos);
        mMenu.update(mCrystals, mInventory, mSkillTree);
    }
    mStartTowerDefenseMenu.update(mCrystals, mAvailableTowers);
}

void RPGEngine::render() {
    std::string currentDate = mTimeSystem.getDateString();
    std::string currentTime = mTimeSystem.getTimeString();

    mWindow.clear();

    sf::View originalView = mWindow.getView();

    if (!mIsInsideAStructure)
        mWindow.setView(mView);
    else
        mWindow.setView(mFixedCamera);

    //mZoneManager.draw(mWindow);

    std::vector<std::pair<float, DrawableEntity*>> renderQueue;

    for (auto& entity : mMap.getEntities()) {
        float depth = entity->getPosition().y + entity->getHeight();

        if (dynamic_cast<MCHouseInt*>(entity.get()))
            depth = -9999.f;
        else if (dynamic_cast<Bed*>(entity.get()))
            depth = -8888.f;

        renderQueue.emplace_back(depth, entity.get());
    }

    renderQueue.emplace_back(mCharacter.getPosition().y + mCharacter.getHeight(), &mCharacter);

    for (auto& npc : mNPCManager.getNPCs())
        renderQueue.emplace_back(npc->getPosition().y + npc->getHeight(), static_cast<DrawableEntity*>(npc.get()));

    for (auto& droppedItem : mDroppedItems)
        renderQueue.emplace_back(droppedItem.getPosition().y + droppedItem.getHeight(), &droppedItem);

    std::sort(renderQueue.begin(), renderQueue.end(),
        [](const std::pair<float, DrawableEntity*>& a, const std::pair<float, DrawableEntity*>& b) {
            return a.first < b.first;
        });

    // Render in order
    for (auto& entity : renderQueue)
        entity.second->render(mWindow);

    for (auto& droppedItem : mDroppedItems)
        droppedItem.render(mWindow);

    if (mShowInteract) {
        mWindow.draw(mInteractCircle);
        mWindow.draw(mInteractText);
    }

    if (mShowDialogue) {
        mWindow.setView(mWindow.getDefaultView());

        sf::RectangleShape dialogueBox(sf::Vector2f(mWindow.getSize().x / 2.8f + 120.f,
            mWindow.getSize().y / 4.2f));
        sf::RectangleShape separationLine(sf::Vector2f(0.2f, dialogueBox.getSize().y));
        separationLine.setFillColor(sf::Color(100, 100, 100, 255));
        dialogueBox.setFillColor(sf::Color(50, 50, 50, 255));
        dialogueBox.setPosition((mWindow.getSize().x - dialogueBox.getSize().x) / 2 - 60.f,
            mWindow.getSize().y - dialogueBox.getSize().y - 35.0f);

        sf::Sprite npcSprite = mCurrentInteractingNPC->getSprite();
        npcSprite.setPosition(dialogueBox.getPosition().x - 21.f,
            dialogueBox.getPosition().y + 37.5f);
        separationLine.setPosition({dialogueBox.getPosition().x + 140.f, dialogueBox.getPosition().y});

        mDialogueInteractCircle.setPosition({dialogueBox.getPosition().x + dialogueBox.getSize().x - 35.f,
            dialogueBox.getPosition().y + 13.f});
        mDialogueInteractText.setPosition({mDialogueInteractCircle.getPosition().x + 9.5f,
            mDialogueInteractCircle.getPosition().y + 6.f});

        mWindow.draw(dialogueBox);
        mWindow.draw(npcSprite);
        mWindow.draw(mDialogueInteractCircle);
        mWindow.draw(mDialogueInteractText);
        mWindow.draw(separationLine);

        float textWidth = dialogueBox.getSize().x - 170.f  - 40.0f;
        std::vector<sf::String> lines = wrapText(mDialogueText.getString(), mFont, mDialogueText.getCharacterSize(), textWidth);

        float lineHeight = mDialogueText.getCharacterSize() * 1.35f;
        float yOffset = dialogueBox.getPosition().y + 10.0f;
        mDialogueText.setPosition(dialogueBox.getPosition().x + 158.f, yOffset);

        for (const auto& line : lines) {
            mDialogueText.setString(line);
            mWindow.draw(mDialogueText);
            yOffset += lineHeight;
            mDialogueText.setPosition(dialogueBox.getPosition().x + 158.f, yOffset);
        }

        if (mNPCManager.currentNPCHasChoices())
            renderDialogueChoices(dialogueBox.getPosition(), dialogueBox.getSize());
    }


    mWindow.setView(mWindow.getDefaultView());

    if (mShowMenu)
        mMenu.render(mWindow);

    if (mShowStartMenu)
        mStartTowerDefenseMenu.render(mWindow);

    if (mShowBankMenu)
        mBankMenu.render(mWindow);

    if (mShowShopMenu)
        mShopMenu.render(mWindow);

    if (mShowAnalyzeMenu)
        mAnalyzeMenu.render(mWindow);

    if (mShowChestMenu)
        mChestMenu.render(mWindow);

    if(!mShowDialogue)
        mHotbar.render(mWindow);

    mTransitionSystem.render(mWindow);

    if (!mShowBankMenu && !mShowMenu && !mShowStartMenu && !mShowShopMenu && !mShowAnalyzeMenu && !mShowChestMenu)
        renderDateTime(mWindow, mFont, currentDate, currentTime);

    mWindow.display();
}

void RPGEngine::renderDialogueChoices(const sf::Vector2f& dialogueBoxPos, const sf::Vector2f& dialogueBoxSize) {
    auto choices = mNPCManager.getCurrentNPCChoices();
    if (choices.empty()) return;

    mChoiceBoxes.clear();
    mChoiceTexts.clear();

    float choiceY = dialogueBoxPos.y + dialogueBoxSize.y + 10.f;
    float choiceWidth = dialogueBoxSize.x;
    float choiceHeight = 35.f;

    for (size_t i = 0; i < choices.size(); ++i) {
        sf::RectangleShape choiceBox;
        choiceBox.setSize({choiceWidth, choiceHeight});
        choiceBox.setPosition(dialogueBoxPos.x, choiceY);

        if (i == mSelectedChoice) {
            choiceBox.setFillColor(sf::Color(70, 70, 70, 255));
            choiceBox.setOutlineColor(sf::Color(150, 150, 150, 150));
            choiceBox.setOutlineThickness(2.f);
        } else {
            choiceBox.setFillColor(sf::Color(40, 40, 40, 255));
            choiceBox.setOutlineColor(sf::Color(100, 100, 100, 155));
            choiceBox.setOutlineThickness(1.f);
        }

        sf::Text choiceText;
        choiceText.setFont(mFont);
        choiceText.setCharacterSize(mDialogueText.getCharacterSize());
        choiceText.setFillColor(sf::Color::White);

        choiceText.setString(choices[i].text);
        choiceText.setPosition(dialogueBoxPos.x + 15.f, choiceY + 8.f);

        mChoiceBoxes.push_back(choiceBox);
        mChoiceTexts.push_back(choiceText);

        mWindow.draw(choiceBox);
        mWindow.draw(choiceText);

        choiceY += choiceHeight + 5.f;
    }
}

void RPGEngine::renderDateTime(sf::RenderWindow& window, sf::Font& font, const std::string& date, const std::string& time) {
    sf::RectangleShape background(sf::Vector2f(200, 80));
    background.setFillColor(sf::Color(0, 0, 0, 150));
    background.setPosition(window.getSize().x - 210.f, 10.f);

    sf::Text dateText;
    dateText.setFont(font);
    dateText.setString(date);
    dateText.setCharacterSize(16);
    dateText.setFillColor(sf::Color::White);
    dateText.setPosition(window.getSize().x - 200.f, 20.f);

    sf::Text timeText;
    timeText.setFont(font);
    timeText.setString(time);
    timeText.setCharacterSize(16);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(window.getSize().x - 200.f, 50.f);

    window.draw(background);
    window.draw(dateText);
    window.draw(timeText);
}

void RPGEngine::resume(int crystals) {
    mCrystals = crystals;
    closeMenues();
}

void RPGEngine::closeMenues() {
    mWindow.setView(mWindow.getDefaultView());
    if (mShowDialogue)
        mShowDialogue = false;
        mNPCManager.resumeCurrentNPC();

    if (mShowMenu)
        mShowMenu = false;

    if (mShowStartMenu)
        mShowStartMenu = false;

    if (mShowBankMenu)
        mShowBankMenu = false;

    if (mShowShopMenu)
        mShowShopMenu = false;

    if (mShowAnalyzeMenu)
        mShowAnalyzeMenu = false;

    if (mShowChestMenu)
        mShowChestMenu = false;
}

void RPGEngine::saveGame() {
    std::vector<sf::Vector2f> npcPositions;
    std::vector<int> npcWaypoints;

    std::vector<int> inventoryItemId;
    std::vector<int> inventoryItemQuantity;
    std::vector<int> chestItemId;
    std::vector<int> chestItemQuantity;
    std::vector<int> droppedItemId;
    std::vector<float> droppedItemXPos;
    std::vector<float> droppedItemYPos;
    std::vector<int> droppedItemQuantity;

    int year = mTimeSystem.getYear();
    int day = mTimeSystem.getDay();
    int hour = mTimeSystem.getHour();
    int minute = mTimeSystem.getMinute();
    int bankBalance = mBankMenu.getBankBalance();

    int hasBorrowActive = (int)mBankMenu.hasActiveBorrow();
    int penalty = mBankMenu.getPenalty();
    int interest = mBankMenu.getInterest();
    int amountToRepay = mBankMenu.getAmounToRepay();
    int daysToRepayment = mBankMenu.getDaysToRepayment();
    int startYear = mBankMenu.getStartYear();
    int startDay = mBankMenu.getStartDay();
    int startHour = mBankMenu.getStartHour();
    int startMinute = mBankMenu.getStartMinute();

    int extracting, inSlot, completed, timerActive, startYear1, startDay1, startHour1, startMinute1, slotItemId;
    mAnalyzeMenu.getInfo(extracting, inSlot, completed, timerActive, startYear1, startDay1, startHour1, startMinute1, slotItemId);

    mNPCManager.saveNPCStates(npcPositions, npcWaypoints);

    for (int ind = 0; ind < mInventory.getSlotCount(); ++ind) {
        if (mInventory.getItemAt(ind)) {
            inventoryItemId.push_back(mInventory.getItemAt(ind)->getId());
            inventoryItemQuantity.push_back(mInventory.getItemQuantityAt(ind));
        }
    }

    for (int ind = 0; ind < mChestInventory.getSlotCount(); ++ind) {
        if (mChestInventory.getItemAt(ind)) {
            chestItemId.push_back(mChestInventory.getItemAt(ind)->getId());
            chestItemQuantity.push_back(mChestInventory.getItemQuantityAt(ind));
        }
    }

    for (auto& droppedItem : mDroppedItems) {
        droppedItemId.push_back(droppedItem.getItem()->getId());
        droppedItemXPos.push_back(droppedItem.getPosition().x);
        droppedItemYPos.push_back(droppedItem.getPosition().y);
        droppedItemQuantity.push_back(droppedItem.getQuantity());
    }

    int chapter = mStoryManager.getChapter();
    std::unordered_map<std::string, bool> flags = mStoryManager.getAllFlags();
    std::vector<std::string> flagKeys;
    std::vector<int> flagValues;

    for (auto& flag : flags) {
        flagKeys.push_back(flag.first);
        flagValues.push_back(flag.second ? 1 : 0);
    }

    /*for (int i = 0; i < inventoryItemId.size(); ++i)
        std::cout << inventoryItemId[i] << " " << inventoryItemQuantity[i] << std::endl;

    for (int i = 0; i < droppedItemId.size(); ++i)
        std::cout << droppedItemId[i] << " " << droppedItemXPos[i] << " " << droppedItemYPos[i] << " " << droppedItemQuantity[i] << std::endl;*/

    mSaveSystem.save(mCharacter.getPosition(), npcPositions, npcWaypoints, mCrystals,
                     year, day, hour, minute, bankBalance, hasBorrowActive, penalty,
                     interest, amountToRepay, daysToRepayment, startYear, startDay,
                     startHour, startMinute, inventoryItemId, inventoryItemQuantity,
                     chestItemId, chestItemQuantity, droppedItemId, droppedItemXPos,
                     droppedItemYPos, droppedItemQuantity, extracting, inSlot, completed,
                     timerActive, startYear1, startDay1, startHour1, startMinute1,
                     slotItemId, mIsInsideAStructure, mCameraFixedPosition, chapter,
                     flagKeys, flagValues);
}

void RPGEngine::loadGame() {
    sf::Vector2f playerPosition;
    std::vector<sf::Vector2f> npcPositions;
    std::vector<int> npcWaypoints;
    int crystals, year, day, hour, minute, bankBalance, penalty, interest,
        amountToRepay, daysToRepayment, startYear, startDay, startHour, startMinute,
        hasBorrowActive, extracting, inSlot, completed, timerActive, startYear1,
        startDay1, startHour1, startMinute1, slotItemId, insideStructure, chapter;
    std::vector<int> droppedItemId;
    std::vector<float> droppedItemXPos;
    std::vector<float> droppedItemYPos;
    std::vector<int> droppedItemQuantity;
    std::vector<int> inventoryItemId;
    std::vector<int> inventoryItemQuantity;
    std::vector<int> chestItemId;
    std::vector<int> chestItemQuantity;
    std::vector<std::string> flagKeys;
    std::vector<int> flagValues;

    if (mSaveSystem.load(playerPosition, npcPositions, npcWaypoints, crystals, year,
                         day, hour, minute, bankBalance, hasBorrowActive, penalty,
                         interest, amountToRepay, daysToRepayment, startYear, startDay,
                         startHour, startMinute, inventoryItemId, inventoryItemQuantity,
                         chestItemId, chestItemQuantity, droppedItemId, droppedItemXPos,
                         droppedItemYPos, droppedItemQuantity, extracting, inSlot,
                         completed, timerActive, startYear1, startDay1, startHour1,
                         startMinute1, slotItemId, insideStructure,
                         mCameraFixedPosition, chapter, flagKeys, flagValues)) {

        mCharacter.setPosition(playerPosition);
        mIsInsideAStructure = insideStructure;
        mNPCManager.loadNPCStates(npcPositions, npcWaypoints);
        mCrystals = crystals;
        mChapter = chapter;
        std::unordered_map<std::string, bool> flags;
        for (size_t i = 0; i < flagKeys.size(); ++i) {
            flags[flagKeys[i]] = (flagValues[i] == 1);
        }
        mStoryManager.setAllFlags(flags);
        mStoryManager.setChapter(chapter);

        mTimeSystem.setYear(year);
        mTimeSystem.setDay(day);
        mTimeSystem.setHour(hour);
        mTimeSystem.setMinute(minute);
        mTimeSystem.resetTimeAccumulator();

        bool borrowActive;
        if (hasBorrowActive)
            borrowActive = true;
        else
            borrowActive = false;

        mBankMenu.setBankBalance(bankBalance);
        mBankMenu.setBorrowStats(borrowActive, penalty, interest, amountToRepay, daysToRepayment, startYear, startDay, startHour, startMinute);
        mAnalyzeMenu.setInfo(extracting, inSlot, completed, timerActive, startYear1, startDay1, startHour1, startMinute1, slotItemId);

        for (size_t i = 0; i < droppedItemId.size(); ++i) {
            const Item* item = nullptr;

            if (droppedItemId[i] == 1)
                item = new Wood();
            else if (droppedItemId[i] == 2)
                item = new TowerBlueprint();
            else if (droppedItemId[i] == 3)
                item = new TowerBlueprintRare();
            else if (droppedItemId[i] == 4)
                item = new TowerBlueprintEpic();
            else if (droppedItemId[i] == 5)
                item = new TowerBlueprintMythic();

            if (item != nullptr) {
                DroppedItem droppedItem(item, sf::Vector2f(droppedItemXPos[i], droppedItemYPos[i]), droppedItemQuantity[i]);
                mDroppedItems.push_back(droppedItem);
            } else
                std::cout << "Error : unknown drop item!" << std::endl;
        }

        for (size_t i = 0; i < inventoryItemId.size(); ++i) {
            if (inventoryItemId[i] == 1) {
                std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
                mInventory.addItem(std::move(woodItem), inventoryItemQuantity[i]);
            } else if (inventoryItemId[i] == 2) {
                std::unique_ptr<TowerBlueprint> towerBlueprint = std::make_unique<TowerBlueprint>();
                mInventory.addItem(std::move(towerBlueprint), inventoryItemQuantity[i]);
            } else if (inventoryItemId[i] == 3) {
                std::unique_ptr<TowerBlueprintRare> towerBlueprintRare = std::make_unique<TowerBlueprintRare>();
                mInventory.addItem(std::move(towerBlueprintRare), inventoryItemQuantity[i]);
            } else if (inventoryItemId[i] == 4) {
                std::unique_ptr<TowerBlueprintEpic> towerBlueprintEpic = std::make_unique<TowerBlueprintEpic>();
                mInventory.addItem(std::move(towerBlueprintEpic), inventoryItemQuantity[i]);
            } else if (inventoryItemId[i] == 5) {
                std::unique_ptr<TowerBlueprintMythic> towerBlueprintMythic = std::make_unique<TowerBlueprintMythic>();
                mInventory.addItem(std::move(towerBlueprintMythic), inventoryItemQuantity[i]);
            }
        }

        for (size_t i = 0; i < chestItemId.size(); ++i) {
            if (chestItemId[i] == 1) {
                std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
                mChestInventory.addItem(std::move(woodItem), chestItemQuantity[i]);
            } else if (chestItemId[i] == 2) {
                std::unique_ptr<TowerBlueprint> towerBlueprint = std::make_unique<TowerBlueprint>();
                mChestInventory.addItem(std::move(towerBlueprint), chestItemQuantity[i]);
            } else if (chestItemId[i] == 3) {
                std::unique_ptr<TowerBlueprintRare> towerBlueprintRare = std::make_unique<TowerBlueprintRare>();
                mChestInventory.addItem(std::move(towerBlueprintRare), chestItemQuantity[i]);
            } else if (chestItemId[i] == 4) {
                std::unique_ptr<TowerBlueprintEpic> towerBlueprintEpic = std::make_unique<TowerBlueprintEpic>();
                mChestInventory.addItem(std::move(towerBlueprintEpic), chestItemQuantity[i]);
            } else if (chestItemId[i] == 5) {
                std::unique_ptr<TowerBlueprintMythic> towerBlueprintMythic = std::make_unique<TowerBlueprintMythic>();
                mChestInventory.addItem(std::move(towerBlueprintMythic), chestItemQuantity[i]);
            }
        }

        /*
        for (int i = 0; i < mInventory.getSlotCount(); ++i) {
            if (mInventory.getItemAt(i))
                std::cout << mInventory.getItemAt(i)->getType() << " " << mInventory.getItemQuantityAt(i) << std::endl;
        }

        for (auto& droppedItem : mDroppedItems) {
            std::cout << droppedItem.getItem()->getType() << " " << droppedItem.getPosition().x << " "
                << droppedItem.getPosition().y << " " << droppedItem.getQuantity() << std::endl;
        }
        */
    } else {
        std::cerr << "Failed to load game data." << std::endl;
    }
}

void RPGEngine::resetSaveGame() {
    std::ofstream saveFile;
    if (mSaveNumber == 1)
        std::ofstream saveFile("save1.txt", std::ofstream::trunc);
    else if (mSaveNumber == 2)
        std::ofstream saveFile("save2.txt", std::ofstream::trunc);
    else if (mSaveNumber == 3)
        std::ofstream saveFile("save3.txt", std::ofstream::trunc);

    if (saveFile.is_open())
        std::cout << "Save file cleared. Game will start from initial positions." << std::endl;
    else
        std::cerr << "Failed to clear save file." << std::endl;

    saveFile.close();

    mCharacter.setPosition(sf::Vector2f(400.f, 300.f));
    mNPCManager.loadNPCStates({}, {});
    mCrystals = 100;
    mChapter = 1;
    mStoryManager.setChapter(1);

    mTimeSystem.setYear(1);
    mTimeSystem.setDay(1);
    mTimeSystem.setHour(6);
    mTimeSystem.setMinute(0);
    mTimeSystem.resetTimeAccumulator();

    mBankMenu.setBankBalance(0);
    mBankMenu.resetBorrowStats();

    mDroppedItems.erase(mDroppedItems.begin(), mDroppedItems.end());

    for (int i = 0; i < mInventory.getSlotCount(); ++i) {
        if (mInventory.getItemAt(i))
            mInventory.removeItemAt(i);
    }
    mAnalyzeMenu.reset();
    mShopMenu.regenerateIds();
    mIsInsideAStructure = false;
}

bool RPGEngine::saveExists(int saveNumber) const {
    if (saveNumber == 1)
        return std::filesystem::exists("save1.txt");
    if (saveNumber == 2)
        return std::filesystem::exists("save2.txt");
    if (saveNumber == 3)
        return std::filesystem::exists("save3.txt");
    return false;
}

void RPGEngine::newGame() {
    if (mSaveNumber == 1) {
        std::ofstream file("save1.txt");
        file.close();
    } else if (mSaveNumber == 2) {
        std::ofstream file("save2.txt");
        file.close();
    } else if (mSaveNumber == 3) {
        std::ofstream file("save3.txt");
        file.close();
    }
}

void RPGEngine::setSaveNumber(int saveNumber) {
    mSaveNumber = saveNumber;
    mSaveSystem.setSaveFilePath(mSaveNumber);
    if (saveExists(saveNumber)) {
        loadGame();
    } else {
        newGame();
        mCharacter.setPosition(sf::Vector2f(340.f, 560.f));
        mCharacter.setAnimation(4);
    }
}

SaveSystem& RPGEngine::getSaveSystem() {
    return mSaveSystem;
}

