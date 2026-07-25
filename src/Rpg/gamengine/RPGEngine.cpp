#include "RPGEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <math.h>
#include "../../utils/utils.h"
#include "../../core/GameManager.h"
#include "../inventory/ItemFactory.h"

RPGEngine::RPGEngine(sf::RenderWindow& window, GameManager* gameManager)
    : mWindow(window),
    mTimeSystem(0.36f),
    mLevelCompleteMenu(sf::Vector2f(window.getSize()), nullptr, this, gameManager, 2, false),
    mCharacter(sf::Vector2f(295.f, 290.f), &mInventory, &mLevelCompleteMenu),
    mView(sf::Vector2f(400.f, 300.f), sf::Vector2f(740.f, 420.f)),
    mFixedCamera(sf::Vector2f(0.f, 0.f), sf::Vector2f(740.f, 420.f)),
    mCameraFixedPosition(sf::Vector2f(0.f, 0.f)),
    mShowDialogue(false),
    mGameManager(gameManager),
    mSaveSystem(),
    mSaveNumber(0),
    mMiraStanton(sf::Vector2f(1000.0f, 900.0f), "mira_stanton"),
    mElliotMarlowe(sf::Vector2f(100.0f, 0.0f), "elliot_marlowe"),
    mGarrickStone(sf::Vector2f(-100.0f, 0.0f), "garrick_stone", gameManager),
    mVincentHale(sf::Vector2f(600.0f, 400.0f), "vincent_hale"),
    mSeraphinaLumeris(sf::Vector2f(-20.0f, 980.0f), "seraphina_lumeris"),
    mNPCManager(mStoryManager, mDialogueDatabase, *this, mInventory),
    mChestInventory(2, 2),
    mCurrentInteractingNPC(nullptr),
    mSkillTree(),
    mCrystals(100),
    mMenu(sf::Vector2f(window.getSize()), mSkillTree, mInventory, *this, gameManager, mCrystals),
    mHotbar(mInventory, sf::Vector2f(mWindow.getSize().x / 2 - 45.f * 3 / 2, mWindow.getSize().y - 45.f - 4.f),
        mInventory.getSlotCount() / 2, sf::Vector2f(45.f, 45.f)),
    mShowInteract(false),
    mIsInsideAStructure(false),
    mStructureIndex(-1),
    mStorageCapacity(500),
    mCurrentTowerDefenseLevel(1),
    mChapter(1),
    mSelectedChoice(0),
    mPaused(true),
    mIsInitialized(false),
    mTransitionSystem(sf::Vector2f(window.getSize().x, window.getSize().y)),
    mChestMenu(sf::Vector2f(window.getSize()), mInventory, mChestInventory, sf::Vector2f(70.f, 70.f)),
    mGameContext{ mCharacter, mIsInsideAStructure, mStructureIndex, mCameraFixedPosition,
                  [this](bool active) { mChestMenu.setActive(active); }, mTimeSystem,  mTransitionSystem, gameManager,
                  [this](const std::string& name) {
                    const Waypoint* waypoint = mWaypointManager.getWaypoint(name);

                    if (waypoint) {
                        mCharacter.setCenterPosition(waypoint->pos);

                        if (waypoint->isInterior) {
                            mIsInsideAStructure = true;
                            mZoneManager.loadInterior(waypoint->mapPath);
                        } else {
                            mIsInsideAStructure = false;
                            mZoneManager.returnToOpenWorld();
                        }
                    }
                  }, mWaypointManager},
    mZoneManager(mGameContext, 1),
    mStartTowerDefenseMenu(sf::Vector2f(window.getSize()), mAvailableTowers, this, gameManager, mCurrentTowerDefenseLevel, mCrystals),
    mBankMenu(sf::Vector2f(window.getSize()), mCrystals, mStorageCapacity, mTimeSystem, gameManager),
    mShopMenu(sf::Vector2f(window.getSize()), mInventory, mTimeSystem, 5, mCrystals, gameManager),
    mAnalyzeMenu(sf::Vector2f(window.getSize()), mInventory, mTimeSystem, gameManager, mAvailableTowers, sf::Vector2f(70.0f, 70.0f), mCrystals) {

    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Couldn't load font from file" << std::endl;

    mDialogueBox.setSize(sf::Vector2f(window.getSize().x / 2.3f, window.getSize().y / 3.8f));
    mDialogueBox.setFillColor(sf::Color(50, 50, 50, 255));
    mDialogueBox.setPosition((window.getSize().x - mDialogueBox.getSize().x) / 2.f, window.getSize().y - mDialogueBox.getSize().y - 15.0f);

    mSeparationLine.setSize(sf::Vector2f(1.f, mDialogueBox.getSize().y));
    mSeparationLine.setFillColor(sf::Color(100, 100, 100, 255));
    mSeparationLine.setPosition(sf::Vector2f(mDialogueBox.getPosition().x + mDialogueBox.getSize().x * 0.25f, mDialogueBox.getPosition().y));

    mDialogueInteractCircle.setRadius(13.f);
    mDialogueInteractCircle.setFillColor(sf::Color(30, 30, 30, 200));
    mDialogueInteractCircle.setOutlineColor(sf::Color::White);
    mDialogueInteractCircle.setOutlineThickness(0.7f);
    mDialogueInteractCircle.setPosition(sf::Vector2f(mDialogueBox.getPosition().x + mDialogueBox.getSize().x - 3.f * mDialogueInteractCircle.getRadius(),
                                                     mDialogueBox.getPosition().y + mDialogueInteractCircle.getRadius()));

    mDialogueInteractText.setFont(mFont);
    mDialogueInteractText.setCharacterSize(10);
    mDialogueInteractText.setFillColor(sf::Color::White);
    mDialogueInteractText.setString("E");
    mDialogueInteractText.setOrigin(mDialogueInteractText.getLocalBounds().left + mDialogueInteractText.getLocalBounds().width / 2.f,
                                    mDialogueInteractText.getLocalBounds().top + mDialogueInteractText.getLocalBounds().height / 2.f);
    mDialogueInteractText.setPosition(sf::Vector2f(mDialogueInteractCircle.getPosition().x + mDialogueInteractCircle.getRadius(),
                                                   mDialogueInteractCircle.getPosition().y + mDialogueInteractCircle.getRadius()));

    mDialogueText.setFont(mFont);
    mDialogueText.setCharacterSize(13);
    mDialogueText.setFillColor(sf::Color::White);
    mDialogueText.setPosition(sf::Vector2f(mSeparationLine.getPosition().x + 15.f, mDialogueBox.getPosition().y + 15.f));

    mDialogueTextWidth = mDialogueBox.getSize().x - mDialogueText.getPosition().x + mDialogueBox.getPosition().x - 5.f * mDialogueInteractCircle.getRadius();
    mDialogueLineHeight = mDialogueText.getCharacterSize() * 1.35f;

    mIconSprite.setPosition(sf::Vector2f((mDialogueBox.getPosition().x + mSeparationLine.getPosition().x) / 2.f,
                                         mDialogueBox.getPosition().y + mDialogueBox.getSize().y / 2.f - 20.f));

    mDateTimeBackground.setSize(sf::Vector2f(200, 80));
    mDateTimeBackground.setFillColor(sf::Color(0, 0, 0, 150));
    mDateTimeBackground.setPosition(window.getSize().x - 210.f, 10.f);

    mDateText.setFont(mFont);
    mDateText.setCharacterSize(16);
    mDateText.setFillColor(sf::Color::White);
    mDateText.setPosition(window.getSize().x - 200.f, 20.f);

    mTimeText.setFont(mFont);
    mTimeText.setCharacterSize(16);
    mTimeText.setFillColor(sf::Color::White);
    mTimeText.setPosition(window.getSize().x - 200.f, 50.f);

    mInteractCircle.setRadius(7.5f);
    mInteractCircle.setFillColor(sf::Color(30, 30, 30, 200));
    mInteractCircle.setOutlineColor(sf::Color::White);
    mInteractCircle.setOutlineThickness(0.7f);

    mInteractText.setFont(mFont);
    mInteractText.setCharacterSize(7);
    mInteractText.setFillColor(sf::Color::White);
    mInteractText.setString("E");
    mInteractText.setOrigin(mInteractText.getLocalBounds().left + mInteractText.getLocalBounds().width / 2.f,
                            mInteractText.getLocalBounds().top + mInteractText.getLocalBounds().height / 2.f);

    //Add NPCs dialogues
    mDialogueDatabase.loadDialogueFromFile("dialogues/mira_stanton.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/seraphina_lumeris.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/garrick_stone.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/vincent_hale.json");
    mDialogueDatabase.loadDialogueFromFile("dialogues/elliot_marlowe.json");

    mWaypointManager.loadWaypoints("assets/maps");

    const Waypoint* spawn = mWaypointManager.getWaypoint("SpawnPoint");

    if (spawn) {
        mCharacter.setCenterPosition(spawn->pos);
        mZoneManager.update(mCharacter.getPosition());
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
                    updateDialogueChoices();
                    return;
                } else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                    mSelectedChoice = (mSelectedChoice + 1) % choices.size();
                    updateDialogueChoices();
                    return;
                } else if (event.key.code == sf::Keyboard::E || event.key.code == sf::Keyboard::Return) {
                    mNPCManager.selectChoiceForCurrentNPC(mSelectedChoice, mShowDialogue, mDialogueText);
                    mSelectedChoice = 0;
                    return;
                }
            }

            if (event.key.code == sf::Keyboard::E || event.key.code == sf::Keyboard::Return) {
                if (!mShowDialogue) {
                    mNPCManager.handleInteraction(mCharacter, mShowDialogue, mDialogueText);

                    if (mShowDialogue) {
                        mCurrentInteractingNPC = mNPCManager.getCurrentNPC();

                        if (mCurrentInteractingNPC)
                            mGameManager->dispatchQuestEvent(GameEvent{ObjectiveType::talk_to_npc, mCurrentInteractingNPC->getId(), 1});

                        mIconSprite = mCurrentInteractingNPC->getAvatarSprite();
                        mIconSprite.setOrigin(mIconSprite.getLocalBounds().left + mIconSprite.getLocalBounds().width / 2.f,
                                              mIconSprite.getLocalBounds().top + mIconSprite.getLocalBounds().height / 2.f);
                        mIconSprite.setPosition(sf::Vector2f((mDialogueBox.getPosition().x + mSeparationLine.getPosition().x) / 2.f,
                                         mDialogueBox.getPosition().y + mDialogueBox.getSize().y / 2.f - 20.f));

                        if (mNPCManager.currentNPCHasChoices()) {
                            mSelectedChoice = 0;
                            updateDialogueChoices();
                        }
                    } else {
                        Entity* interactable = mZoneManager.checkInteraction(mCharacter.getInteractBounds());
                        if (interactable)
                            interactable->interact();
                    }
                } else if (mCurrentInteractingNPC && mShowDialogue) {
                    if (!mNPCManager.currentNPCHasChoices()) {
                        mNPCManager.interactWithCurrentNPC(mShowDialogue, mDialogueText);

                        if (mNPCManager.currentNPCHasChoices()) {
                            mSelectedChoice = 0;
                            updateDialogueChoices();
                        }
                    }
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
                } else if (mStartTowerDefenseMenu.isActive()) {
                    mStartTowerDefenseMenu.setActive(false);
                } else if (mBankMenu.isActive()) {
                    mBankMenu.setActive(false);
                    mBankMenu.restart();
                } else if (mAnalyzeMenu.isActive()) {
                    mAnalyzeMenu.setActive(false);
                } else if (mChestMenu.isActive()) {
                    mChestMenu.setActive(false);
                } else if (mShopMenu.isActive()) {
                    mShopMenu.setActive(false);
                } else {
                    if (!mMenu.isActive())
                        mMenu.restart();

                    mMenu.toggle();
                }
            } else if (event.key.code == sf::Keyboard::L) {
                mStartTowerDefenseMenu.toggle();

                if (mStartTowerDefenseMenu.isActive()) {
                    mMenu.setActive(false);
                    mBankMenu.setActive(false);
                    mShopMenu.setActive(false);
                    mAnalyzeMenu.setActive(false);
                    mChestMenu.setActive(false);

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
                mBankMenu.toggle();

                if (mBankMenu.isActive()) {
                    mMenu.setActive(false);
                    mStartTowerDefenseMenu.setActive(false);
                    mShopMenu.setActive(false);
                    mAnalyzeMenu.setActive(false);
                    mChestMenu.setActive(false);

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
                mShopMenu.toggle();

                if (mShopMenu.isActive()) {
                    if (mShowDialogue) {
                        mShowDialogue = false;
                        if (mCurrentInteractingNPC) {
                            mCurrentInteractingNPC->resetDialogue();
                            mCurrentInteractingNPC->resumeMovement();
                            mCurrentInteractingNPC = nullptr;
                        }
                    }

                    mMenu.setActive(false);
                    mBankMenu.setActive(false);
                    mStartTowerDefenseMenu.setActive(false);
                    mAnalyzeMenu.setActive(false);
                    mChestMenu.setActive(false);
                }
            } else if (event.key.code == sf::Keyboard::Q) {
                if (mMenu.isActive() && mMenu.getMenuType() == "Inventory") {
                    int slotIndex = mMenu.getInventoryMenu().getHoveredSlot();
                    if (slotIndex != -1) {
                        if (mInventory.getItemAt(slotIndex)) {
                            int quantity = mInventory.getItemQuantityAt(slotIndex);
                            std::unique_ptr<Item> uniqueItem = mInventory.extractItemAt(slotIndex);
                            std::shared_ptr<const Item> sharedItem(std::move(uniqueItem));

                            const DroppedItem dropItem(sharedItem, calculateDropPosition(), quantity);
                            mDroppedItems.push_back(std::move(dropItem));
                        }
                    }
                }

                if (!mMenu.isActive() && !mBankMenu.isActive() && !mStartTowerDefenseMenu.isActive() && !mShowDialogue && !mAnalyzeMenu.isActive() && !mChestMenu.isActive()) {
                    int slotIndex = mHotbar.getHoveredSlot();
                    if (slotIndex != -1) {
                        if (mInventory.getItemAt(slotIndex)) {
                            int quantity = mInventory.getItemQuantityAt(slotIndex);
                            std::unique_ptr<Item> uniqueItem = mInventory.extractItemAt(slotIndex);
                            std::shared_ptr<const Item> sharedItem(std::move(uniqueItem));

                            const DroppedItem dropItem(sharedItem, calculateDropPosition(), quantity);
                            mDroppedItems.push_back(std::move(dropItem));
                        }
                    }
                }
            } else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Num3) {
                if (!mBankMenu.isActive() && !mShowDialogue && !mMenu.isActive() && !mStartTowerDefenseMenu.isActive() && !mAnalyzeMenu.isActive() && !mChestMenu.isActive()) {
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
                mAnalyzeMenu.toggle();

                if (mAnalyzeMenu.isActive()) {
                    mMenu.setActive(false);
                    mStartTowerDefenseMenu.setActive(false);
                    mShopMenu.setActive(false);
                    mBankMenu.setActive(false);
                    mChestMenu.setActive(false);

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

                if (mShowDialogue) {
                    if (mNPCManager.currentNPCHasChoices()) {
                        for (size_t i = 0; i < mChoiceBoxes.size(); ++i) {
                            if (mChoiceBoxes[i].getGlobalBounds().contains(mousePos)) {
                                mNPCManager.selectChoiceForCurrentNPC(i, mShowDialogue, mDialogueText);
                                mSelectedChoice = 0;
                                return;
                            }
                        }
                    } else {
                        mNPCManager.interactWithCurrentNPC(mShowDialogue, mDialogueText);
                        if (mNPCManager.currentNPCHasChoices()) {
                            mSelectedChoice = 0;
                            updateDialogueChoices();
                        }
                        return;
                    }
                }

                mMenu.handleMouseClick(mousePos);
                mStartTowerDefenseMenu.handleMouseClick(mousePos);
                mBankMenu.handleMouseClick(mousePos);
                mShopMenu.handleMouseClick(mousePos);
                mAnalyzeMenu.handleMouseClick(mousePos);
                mChestMenu.handleMouseClick(mousePos);

                mLevelCompleteMenu.handleMouseClick(mousePos);

                if(!mMenu.isActive() && !mStartTowerDefenseMenu.isActive() && !mBankMenu.isActive() && !mShopMenu.isActive() && !mAnalyzeMenu.isActive() && !mChestMenu.isActive() && !mShowDialogue) {
                    int slot = mHotbar.contains(mousePos);
                    mHotbar.setHoveredSlot(slot);
                }
            }
        }
    }
}

void RPGEngine::update(float dt) {
    if (mPaused)
        return;

    sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));

    if (mMenu.isActive()) {
        mMenu.updateHover(mousePos);
        mMenu.update(dt);
        return;
    }

    if (mStartTowerDefenseMenu.isActive()) {
        mStartTowerDefenseMenu.updateHover(mousePos);
        mStartTowerDefenseMenu.update(dt);
        return;
    }

    if (mBankMenu.isActive()) {
        mBankMenu.updateHover(mousePos);
        mBankMenu.update(dt);
        return;
    }

    if (mShopMenu.isActive()) {
        mShopMenu.updateHover(mousePos);
        mShopMenu.update(dt);
        return;
    }

    if (mAnalyzeMenu.isActive()) {
        mAnalyzeMenu.updateHover(mousePos);
        mAnalyzeMenu.update(dt);
        return;
    }

    if (mChestMenu.isActive()) {
        mChestMenu.updateHover(mousePos);
        return;
    }

    if (mLevelCompleteMenu.isActive()) {
        mLevelCompleteMenu.updateHover(mousePos);
        return;
    }

    sf::Vector2f previousPosition = mCharacter.getPosition();

    if (!mShowDialogue)
        mHotbar.update();

    mTransitionSystem.update(dt);

    mTimeSystem.update(dt);

    if (mTimeSystem.getHour() == 0 && mTimeSystem.getMinute() == 0)
        mShopMenu.regenerateIds();

    if (mTransitionSystem.isTransitioning())
        return;

    mCharacter.update(dt, mShowDialogue);
    mZoneManager.update(mCharacter.getPosition());

    if (mZoneManager.checkCollision(mCharacter.getBounds()))
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

    if (mCameraFixedPosition.x != 0.f || mCameraFixedPosition.y != 0.f)
        mFixedCamera.setCenter(mCameraFixedPosition);
    else
        mFixedCamera.setCenter(mCharacter.getCenterPosition());

    if (!mNPCManager.playerClose(mCharacter.getPosition()))
        mShowDialogue = false;

    bool mouseMoved = (mousePos != mLastMousePos);
    mLastMousePos = mousePos;

    if (mShowDialogue && mNPCManager.currentNPCHasChoices()) {
        if (mouseMoved) {
            bool changed = false;

            for (int i = 0; i < mChoiceBoxes.size(); ++i) {
                if (mChoiceBoxes[i].getGlobalBounds().contains(mousePos) && mSelectedChoice != i) {
                    mSelectedChoice = i;
                    changed = true;
                }
            }

            if (changed)
                updateDialogueChoices();
        }
    }

    sf::FloatRect playerBounds = mCharacter.getBounds();
    sf::Vector2f playerCenter = {playerBounds.left + playerBounds.width / 2.f, playerBounds.top + playerBounds.height / 2.f};

    for (auto it = mDroppedItems.begin(); it != mDroppedItems.end();) {
        sf::Vector2f itemCenter = it->getCenterPosition();
        float distance = std::sqrt(
            std::pow(playerCenter.x - itemCenter.x, 2.f) +
            std::pow(playerCenter.y - itemCenter.y, 2.f)
        );

        if (it->getPickUpCap()) {
            if (distance < 28.f) {
                auto itemClone = it->getItem()->clone();

                mGameManager->dispatchQuestEvent(GameEvent{ObjectiveType::get_item, std::to_string(itemClone->getId()), it->getQuantity()});

                mInventory.addItem(std::move(itemClone), it->getQuantity());
                it = mDroppedItems.erase(it);
                continue;
            }
        } else {
            if (distance > 28.f)
                it->setPickUpCap(true);
        }
        ++it;
    }

    mShowInteract = false;

    for (auto& entity : mZoneManager.getEntities()) {
        if (!entity->isInteractable())
            continue;

        if (entity->getInteractBounds().intersects(mCharacter.getInteractBounds())) {
            mShowInteract = true;

            mInteractPos = entity->getInteractPosition();

            mInteractCircle.setPosition(sf::Vector2f(mInteractPos.x + entity->getInteractBounds().width + 1.5f, mInteractPos.y - 12.f));
            mInteractText.setPosition(sf::Vector2f(mInteractCircle.getPosition().x + mInteractCircle.getRadius(),
                                                   mInteractCircle.getPosition().y + mInteractCircle.getRadius()));

            break;
        }
    }

    if (!mShowInteract) {
        mInteractPos = { 0.f, 0.f };
        mInteractCircle.setPosition(mInteractPos);
        mInteractText.setPosition(mInteractPos);
    }
}

void RPGEngine::render() {
    if (!mIsInsideAStructure)
        mWindow.setView(mView);
    else
        mWindow.setView(mFixedCamera);

    mZoneManager.render(mWindow);

    mRenderQueue.clear();

    for (auto* entity: mZoneManager.getEntities()) {
        mRenderQueue.push_back(entity);
    }

    for (auto& npc : mNPCManager.getNPCs())
        mRenderQueue.push_back(npc.get());

    for (auto& droppedItem : mDroppedItems)
        mRenderQueue.push_back(&droppedItem);

    mRenderQueue.push_back(&mCharacter);

    std::sort(mRenderQueue.begin(), mRenderQueue.end(), [&](DrawableEntity* a, DrawableEntity* b) {
        float depthA = a->getDepthOffset();
        float depthB = b->getDepthOffset();

        if (depthA == 0.f)
            depthA = a->getPosition().y + a->getHeight();
        if (depthB == 0.f)
            depthB = b->getPosition().y + b->getHeight();

        return depthA < depthB;
    });

    // Render in order
    for (auto& entity : mRenderQueue)
        entity->render(mWindow);

    if (mShowInteract) {
        mWindow.draw(mInteractCircle);
        mWindow.draw(mInteractText);
    }

    mWindow.setView(mWindow.getDefaultView());

    if (mShowDialogue) {
        mWindow.draw(mDialogueBox);
        mWindow.draw(mSeparationLine);
        mWindow.draw(mIconSprite);
        mWindow.draw(mDialogueInteractCircle);
        mWindow.draw(mDialogueInteractText);

        auto lines = wrapText(mDialogueText.getString(), mFont, mDialogueText.getCharacterSize(), mDialogueTextWidth);
        float yOffset = mDialogueBox.getPosition().y + 15.f;

        mDialogueText.setPosition(sf::Vector2f(mSeparationLine.getPosition().x + 15.f, yOffset));

        sf::Text lineText = mDialogueText;
        for (const auto& line : lines) {
            lineText.setPosition(sf::Vector2f(mSeparationLine.getPosition().x + 15.f, yOffset));
            lineText.setString(line);
            mWindow.draw(lineText);
            yOffset += mDialogueLineHeight;
        }

        if (mNPCManager.currentNPCHasChoices())
            renderDialogueChoices();
    }

    mMenu.render(mWindow);
    mStartTowerDefenseMenu.render(mWindow);
    mBankMenu.render(mWindow);
    mShopMenu.render(mWindow);
    mAnalyzeMenu.render(mWindow);
    mChestMenu.render(mWindow);

    if(!mShowDialogue)
        mHotbar.render(mWindow);

    mLevelCompleteMenu.render(mWindow);
    mTransitionSystem.render(mWindow);

    mDateText.setString(mTimeSystem.getDateString());
    mTimeText.setString(mTimeSystem.getTimeString());
    mWindow.draw(mDateTimeBackground);
    mWindow.draw(mDateText);
    mWindow.draw(mTimeText);
}

void RPGEngine::renderDialogueChoices() {
    for (const auto& box: mChoiceBoxes)
        mWindow.draw(box);

    for (const auto& text: mChoiceTexts)
        mWindow.draw(text);
}

void RPGEngine::updateDialogueChoices() {
    auto choices = mNPCManager.getCurrentNPCChoices();
    if (choices.empty()) return;

    mChoiceBoxes.resize(choices.size());
    mChoiceTexts.resize(choices.size());

    float choiceWidth = mDialogueBox.getSize().x;
    float choiceHeight = 35.f;
    float spacing = 5.f;

    float totalHeight = (choiceHeight + spacing) * choices.size();
    float startY = mDialogueBox.getPosition().y - totalHeight - 10.f;

    for (size_t i = 0; i < choices.size(); ++i) {
        float choiceY = startY + i * (choiceHeight + spacing);

        mChoiceBoxes[i].setSize(sf::Vector2f(choiceWidth, choiceHeight));
        mChoiceBoxes[i].setPosition(sf::Vector2f(mDialogueBox.getPosition().x, choiceY));

        if (i == mSelectedChoice) {
            mChoiceBoxes[i].setFillColor(sf::Color(70, 70, 70, 255));
            mChoiceBoxes[i].setOutlineColor(sf::Color(150, 150, 150, 150));
            mChoiceBoxes[i].setOutlineThickness(2.f);
        } else {
            mChoiceBoxes[i].setFillColor(sf::Color(40, 40, 40, 255));
            mChoiceBoxes[i].setOutlineColor(sf::Color(100, 100, 100, 155));
            mChoiceBoxes[i].setOutlineThickness(1.f);
        }

        mChoiceTexts[i].setFont(mFont);
        mChoiceTexts[i].setCharacterSize(mDialogueText.getCharacterSize());
        mChoiceTexts[i].setFillColor(sf::Color::White);

        mChoiceTexts[i].setString(choices[i].text);
        mChoiceTexts[i].setPosition(mDialogueBox.getPosition().x + 15.f, choiceY + 8.f);
    }
}

void RPGEngine::resume(int crystals) {
    mCrystals = crystals;
    closeMenues();
    mPaused = false;

    mClock.restart();

    if (mIsInsideAStructure) {
        mFixedCamera.setCenter(mCameraFixedPosition);
        mWindow.setView(mFixedCamera);
    } else {
        mView.setCenter(mCharacter.getPosition());
        mWindow.setView(mView);
    }
}

void RPGEngine::enterRPG() {
    closeMenues();
    mMenu.switchToMenu("Inventory");
    mPaused = false;

    mClock.restart();

    if (mIsInsideAStructure) {
        mFixedCamera.setCenter(mCameraFixedPosition);
        mWindow.setView(mFixedCamera);
    } else {
        mView.setCenter(mCharacter.getPosition());
        mWindow.setView(mView);
    }
}

void RPGEngine::exitRPG() {
    saveGame();
    mPaused = true;
}

void RPGEngine::closeMenues() {
    mWindow.setView(mWindow.getDefaultView());
    if (mShowDialogue)
        mShowDialogue = false;
        mNPCManager.resumeCurrentNPC();

    mMenu.setActive(false);
    mStartTowerDefenseMenu.setActive(false);
    mBankMenu.setActive(false);
    mShopMenu.setActive(false);
    mAnalyzeMenu.setActive(false);
    mChestMenu.setActive(false);
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
        if (const Item* item = droppedItem.getItem()) {
            droppedItemId.push_back(droppedItem.getItem()->getId());
            droppedItemXPos.push_back(droppedItem.getPosition().x);
            droppedItemYPos.push_back(droppedItem.getPosition().y);
            droppedItemQuantity.push_back(droppedItem.getQuantity());
        }
    }

    int chapter = mStoryManager.getChapter();
    std::unordered_map<std::string, bool> flags = mStoryManager.getAllFlags();
    std::vector<std::string> flagKeys;
    std::vector<int> flagValues;

    for (auto& flag : flags) {
        flagKeys.push_back(flag.first);
        flagValues.push_back(flag.second ? 1 : 0);
    }

    std::vector<std::string> questIds;
    std::vector<int> questStates;
    std::vector<std::vector<int>> questObjectives;

    if (mGameManager)
        mGameManager->getQuestManager().getQuestSaveData(questIds, questStates, questObjectives);

    /*for (int i = 0; i < inventoryItemId.size(); ++i)
        std::cout << inventoryItemId[i] << " " << inventoryItemQuantity[i] << std::endl;

    for (int i = 0; i < droppedItemId.size(); ++i)
        std::cout << droppedItemId[i] << " " << droppedItemXPos[i] << " " << droppedItemYPos[i] << " " << droppedItemQuantity[i] << std::endl;*/

    mSaveSystem.save(mCharacter.getPosition(), mCharacter.getAnimation(), mCharacter.getLevel(), mCharacter.getXp(),
                     mCurrentTowerDefenseLevel, npcPositions, npcWaypoints, mCrystals,
                     year, day, hour, minute, bankBalance, hasBorrowActive, penalty,
                     interest, amountToRepay, daysToRepayment, startYear, startDay,
                     startHour, startMinute, inventoryItemId, inventoryItemQuantity,
                     chestItemId, chestItemQuantity, droppedItemId, droppedItemXPos,
                     droppedItemYPos, droppedItemQuantity, extracting, inSlot, completed,
                     timerActive, startYear1, startDay1, startHour1, startMinute1,
                     slotItemId, mIsInsideAStructure, mStructureIndex, mCameraFixedPosition,
                     chapter, flagKeys, flagValues, questIds, questStates, questObjectives);
}

void RPGEngine::loadGame() {
    sf::Vector2f playerPosition;
    int playerAnimation, playerLevel, playerXp;
    int towerDefenseLevel;
    std::vector<sf::Vector2f> npcPositions;
    std::vector<int> npcWaypoints;

    int crystals, year, day, hour, minute, bankBalance, penalty, interest,
        amountToRepay, daysToRepayment, startYear, startDay, startHour, startMinute,
        hasBorrowActive, extracting, inSlot, completed, timerActive, startYear1,
        startDay1, startHour1, startMinute1, slotItemId, insideStructure, structureIndex, chapter;

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

    std::vector<std::string> questIds;
    std::vector<int> questStates;
    std::vector<std::vector<int>> questObjectives;

    if (mSaveSystem.load(playerPosition, playerAnimation, playerLevel, playerXp,
                         towerDefenseLevel, npcPositions, npcWaypoints, crystals, year,
                         day, hour, minute, bankBalance, hasBorrowActive, penalty,
                         interest, amountToRepay, daysToRepayment, startYear, startDay,
                         startHour, startMinute, inventoryItemId, inventoryItemQuantity,
                         chestItemId, chestItemQuantity, droppedItemId, droppedItemXPos,
                         droppedItemYPos, droppedItemQuantity, extracting, inSlot,
                         completed, timerActive, startYear1, startDay1, startHour1,
                         startMinute1, slotItemId, insideStructure, structureIndex,
                         mCameraFixedPosition, chapter, flagKeys, flagValues,
                         questIds, questStates, questObjectives)) {

        if (mGameManager)
            mGameManager->getQuestManager().loadQuestSaveData(questIds, questStates, questObjectives);

        mInventory.clear();
        mChestInventory.clear();
        mDroppedItems.clear();

        mCharacter.setAnimation(playerAnimation);
        mCharacter.setLevel(playerLevel);
        mCharacter.setXp(playerXp);
        mCurrentTowerDefenseLevel = towerDefenseLevel;

        mIsInsideAStructure = insideStructure;
        mStructureIndex = structureIndex;
        if (mIsInsideAStructure == true) {
            if (mStructureIndex == 0)
                mGameContext.changeMap("MCHouse_Interior");
            mCharacter.setPosition(playerPosition);
        } else {
            mGameContext.changeMap("open_world");
            mStructureIndex = -1;
        }

        mNPCManager.loadNPCStates(npcPositions, npcWaypoints);
        mCrystals = crystals;

        mChapter = chapter;
        std::unordered_map<std::string, bool> flags;
        for (size_t i = 0; i < flagKeys.size(); ++i)
            flags[flagKeys[i]] = (flagValues[i] == 1);

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
            auto uniqueItem = ItemFactory::createItemById(droppedItemId[i]);
            if (!uniqueItem) {
                std::cerr << "Error: unknown dropped item ID " << droppedItemId[i] << std::endl;
                continue;
            }

            std::shared_ptr<const Item> sharedItem(std::move(uniqueItem));

            DroppedItem dropped(sharedItem,
                { droppedItemXPos[i], droppedItemYPos[i] },
                droppedItemQuantity[i]);

            mDroppedItems.push_back(std::move(dropped));
        }

        for (size_t i = 0; i < inventoryItemId.size(); ++i) {
            auto item = ItemFactory::createItemById(inventoryItemId[i]);
            if (item)
                mInventory.addItem(std::move(item), inventoryItemQuantity[i]);
            else
                std::cerr << "Unknown inventory item ID " << inventoryItemId[i] << std::endl;
        }

        for (size_t i = 0; i < chestItemId.size(); ++i) {
            auto item = ItemFactory::createItemById(chestItemId[i]);
            if (item)
                mChestInventory.addItem(std::move(item), chestItemQuantity[i]);
            else
                std::cerr << "Unknown chest item ID " << chestItemId[i] << std::endl;
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
    std::string filename;
    if (mSaveNumber == 1)
        filename = "save1.txt";
    else if (mSaveNumber == 2)
        filename = "save2.txt";
    else if (mSaveNumber == 3)
        filename = "save3.txt";
    else
        return;

    std::ofstream saveFile(filename, std::ofstream::trunc);
    if (saveFile.is_open()) {
        std::cout << "Save file cleared. Game will start from initial positions." << std::endl;
        saveFile.close();
    } else
        std::cerr << "Failed to clear save file." << std::endl;
    
    resetToDefault();
}

void RPGEngine::resetToDefault() {
    const Waypoint* spawn = mWaypointManager.getWaypoint("SpawnPoint");

    if (spawn) {
        mCharacter.setCenterPosition(spawn->pos);
        mZoneManager.update(mCharacter.getPosition());
    }

    mCharacter.setAnimation(4);
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

    mAnalyzeMenu.reset();
    mShopMenu.regenerateIds();
    mIsInsideAStructure = false;
    mCameraFixedPosition = sf::Vector2f(0.f, 0.f);
    mMenu.switchToMenu("Inventory");

    if (mGameManager) {
        mGameManager->getQuestManager().resetQuests();
        mGameManager->getQuestManager().startQuest("quest_1");
        mGameManager->getNotificationManager().addNotification(NotificationManager::Type::QuestStarted, "The First Meeting");
    }
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

bool RPGEngine::deleteSave(int saveNumber) {
    std::string filename;
    if (saveNumber == 1)
        filename = "save1.txt";
    else if (saveNumber == 2)
        filename = "save2.txt";
    else if (saveNumber == 3)
        filename = "save3.txt";
    else
        return false;

    std::error_code error;
    const bool removed = std::filesystem::remove(filename, error);
    if (!removed && error) {
        std::cerr << "Failed to delete save file: " << filename << std::endl;
        return false;
    }

    return true;
}

void RPGEngine::newGame() {
    std::string filename;
    if (mSaveNumber == 1)
        filename = "save1.txt";
    else if (mSaveNumber == 2)
        filename = "save2.txt";
    else if (mSaveNumber == 3)
        filename = "save3.txt";
    else
        return;

    std::ofstream file(filename);
    file.close();

    resetToDefault();

    mInventory.addItem(std::make_unique<Wood>(), 1);
    mInventory.addItem(std::make_unique<TowerBlueprint>(), 5);
    mInventory.addItem(std::make_unique<TowerBlueprintEpic>(), 2);

    saveGame();
}

void RPGEngine::setSaveNumber(int saveNumber) {
    if (mSaveNumber != 0 && mSaveNumber != saveNumber && mIsInitialized) {
        saveGame();
        uninitialize();
    }

    mSaveNumber = saveNumber;
    mSaveSystem.setSaveFilePath(mSaveNumber);
    
    if (!mIsInitialized)
        initialize();
    
    if (saveExists(saveNumber))
        loadGame();
    else
        newGame();
}

void RPGEngine::setFlag(std::string name, bool value) {
    if (name == "mShowMenu")
        mMenu.setActive(value);
    else if (name == "mShowStartMenu")
        mStartTowerDefenseMenu.setActive(value);
    else if (name == "mShowBankMenu")
        mBankMenu.setActive(value);
    else if (name == "mShowShopMenu")
        mShopMenu.setActive(value);
    else if (name == "mShowAnalyzeMenu")
        mAnalyzeMenu.setActive(value);
    else if (name == "mShowChestMenu")
        mChestMenu.setActive(value);
}

void RPGEngine::addCrystals(int value) {
    mCrystals += value;
}

void RPGEngine::setCrystals(int crystals) {
    mCrystals = crystals;
}

void RPGEngine::initialize() {
    if (mIsInitialized)
        return;

    mNPCManager.clearAllNPCs();
    mNPCManager.addNPC(std::make_unique<GarrickStone>(sf::Vector2f(650.0f, 400.0f), "garrick_stone", mGameManager));
    mNPCManager.addNPC(std::make_unique<MiraStanton>(sf::Vector2f(1000.0f, 900.0f), "mira_stanton"));
    mNPCManager.addNPC(std::make_unique<ElliotMarlowe>(sf::Vector2f(100.0f, 0.0f), "elliot_marlowe"));
    mNPCManager.addNPC(std::make_unique<VincentHale>(sf::Vector2f(-100.0f, 0.0f), "vincent_hale"));
    mNPCManager.addNPC(std::make_unique<SeraphinaLumeris>(sf::Vector2f(-20.0f, 980.0f), "seraphina_lumeris"));
    mStoryManager.bindNPCManager(&mNPCManager);

    mAvailableTowers.clear();
    mAvailableTowers.push_back(2);
    mAvailableTowers.push_back(1);
    mAvailableTowers.push_back(3);

    mIsInitialized = true;
}

void RPGEngine::uninitialize() {
    if (!mIsInitialized)
        return;

    mNPCManager.clearAllNPCs();
    mAvailableTowers.clear();

    mInventory.clear();
    mChestInventory.clear();
    mDroppedItems.clear();

    mTimeSystem.reset();
    mIsInsideAStructure = false;

    mIsInitialized = false;
}

sf::Vector2f RPGEngine::calculateDropPosition() {
    sf::FloatRect playerBounds = mCharacter.getBounds();
    sf::Vector2f startPos = {playerBounds.left + playerBounds.width / 2.f, playerBounds.top + playerBounds.height / 2.f};

    sf::Vector2f dir(0.f, 0.f);
    int playerAnimation = mCharacter.getAnimation();
    if (playerAnimation == 1) {
        dir = {-1.f, 0.f};
        startPos.x += 5.f;
        startPos.y -= 4.f;
    } else if (playerAnimation == 2) {
        dir = {1.f, 0.f};
        startPos.x -= 6.f;
        startPos.y -= 5.f;
    } else if (playerAnimation == 3) {
        dir = {0.f, -1.f};
    } else if (playerAnimation == 4) {
        dir = {0.f, 1.f};
        startPos.y -= 7.f;
    }

    float maxDist = 28.f;
    float step = 2.f;
    float itemSize = 22.f;
    sf::Vector2f validPos = startPos;

    for (float dist = itemSize; dist <= maxDist; dist += step) {
        sf::Vector2f nextPos = startPos + (dir * dist);

        sf::FloatRect itemBounds(
            nextPos.x - itemSize / 2.f,
            nextPos.y - itemSize / 2.f,
            itemSize,
            itemSize
        );

        if (mZoneManager.checkCollision(itemBounds))
            break;

        validPos = nextPos;
    }

    return validPos - sf::Vector2f(8.f, 8.f);
}

void RPGEngine::addXp(int xp) {
    mCharacter.getLevelSystem().addXp(xp);
}

void RPGEngine::advanceTowerDefenseLevel() {
    mStartTowerDefenseMenu.advanceLevel();
}

SaveSystem& RPGEngine::getSaveSystem() {
    return mSaveSystem;
}

Inventory& RPGEngine::getInventory() {
    return mInventory;
}

MainCharacter& RPGEngine::getPlayer() {
    return mCharacter;
}

void RPGEngine::rewardItem(const std::string& itemName, int amount) {
    if (itemName == "wood")
        mInventory.addItem(std::make_unique<Wood>(), amount);
    else if (itemName == "tower_blueprint")
        mInventory.addItem(std::make_unique<TowerBlueprint>(), amount);
    else if (itemName == "tower_blueprint_rare")
        mInventory.addItem(std::make_unique<TowerBlueprintRare>(), amount);
    else if (itemName == "tower_blueprint_epic")
        mInventory.addItem(std::make_unique<TowerBlueprintEpic>(), amount);
    else if (itemName == "tower_blueprint_mythic")
        mInventory.addItem(std::make_unique<TowerBlueprintMythic>(), amount);
}
