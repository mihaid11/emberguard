#include "RPGEngine.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include "../../GameManager.h"

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
			// Otherwise, add the word to the current line
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

static bool intersects(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
	return rect1.intersects(rect2);
}

RPGEngine::RPGEngine(sf::RenderWindow& window, GameManager* gameManager)
	: mWindow(window),
	mMap(),
	mTimeSystem(1.2f),
	mCharacter(sf::Vector2f(400.f, 300.f), mMap),
	mView(sf::Vector2f(400.f, 300.f), sf::Vector2f(1280.f, 720.f)),
	mFixedCamera(sf::Vector2f(0.f, 0.f), sf::Vector2f(1280.f, 720.f)),
	mCameraFixedPosition(sf::Vector2f(0.f, 0.f)),
	mShowDialogue(false),
	mGameManager(gameManager),
	mSaveSystem("../../savegame.txt"),
	mMiraStanton(sf::Vector2f(1000.0f, 900.0f)),
	mElliotMarlowe(sf::Vector2f(100.0f, 0.0f)),
	mGarrickStone(sf::Vector2f(-100.0f, 0.0f), gameManager),
	mVincentHale(sf::Vector2f(600.0f, 400.0f)),
	mSeraphinaLumeris(sf::Vector2f(-20.0f, 980.0f)),
	mNPCManager(),
	mCurrentInteractingNPC(nullptr),
	mSkillTree(),
	mMenu(window, mSkillTree, mInventory, mCharacter.getPosition(), mDroppedItems, *this),
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
	//mZoneManager(),
	mStartTowerDefenseMenu(window, mAvailableTowers, this, gameManager, mCurrentLevel, mCrystals),
	mBankMenu(window, mCrystals, mStorageCapacity, mTimeSystem),
	mShopMenu(window, mInventory, mCrystals),
	mAnalyzeMenu(window, mInventory, mTimeSystem, mAvailableTowers, sf::Vector2f(70.0f, 70.0f), mCrystals)
	{

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
	mMap.addEntity<Barrier>(sf::Vector2f(-1004.f, -920.f), sf::Vector2f(30.f, 420.f));
	mMap.addEntity<Barrier>(sf::Vector2f(-443.5f, -920.f), sf::Vector2f(30.f, 260.f)); 
	mMap.addEntity<Barrier>(sf::Vector2f(-1000.f, -553.5f), sf::Vector2f(105.f, 25.f));
	mMap.addEntity<Barrier>(sf::Vector2f(-758.f, -685.f), sf::Vector2f(325.f, 25.f));
	mMap.addEntity<Barrier>(sf::Vector2f(-838.f, -553.5f), sf::Vector2f(107.f, 25.f));
	mMap.addEntity<Barrier>(sf::Vector2f(-759.5f, -685.f), sf::Vector2f(30.f, 130.f));
	mMap.addEntity<Barrier>(sf::Vector2f(-897.f, -520.f), sf::Vector2f(60.f, 20.f));

	// MCHouseInt bed
	mMap.addEntity<Bed>(sf::Vector2f(-544.f, -833.f), 1.65f, "assets/sprites/buildings/bed.png",
		sf::Vector2f(-539.f, -838.f), sf::Vector2f(75.2f, 1.f),
		sf::Vector2f(-546.f, -839.f), sf::Vector2f(85.f, 41.f), mTimeSystem,
		gameManager);

	mMap.addEntity<MCHouse>(sf::Vector2f(300.f, 300.f), "assets/sprites/buildings/mcHouseExt.png",
		sf::Vector2f(313.f, 481.f), sf::Vector2f(143.f, 30.f), 1, sf::Vector2f(340.f, 505.f),
		sf::Vector2f(60.f, 30.f), mCharacter, mIsInsideAStructure, mCameraFixedPosition);
	mMap.addEntity<MCHouseInt>(sf::Vector2f(-1000.f, -1000.f), "assets/sprites/buildings/mcHouseInt.png",
		sf::Vector2f(-980.f, -920.f), sf::Vector2f(580.f, 40.f), 1, sf::Vector2f(-894.f, -530.f),
		sf::Vector2f(48.f, 18.f), mCharacter, mIsInsideAStructure, mCameraFixedPosition);

	//mZoneManager.loadTileset("assets/sprites/tiles/Tileset.png");

	// Add NPCs
	mNPCManager.addNPC(std::make_unique<GarrickStone>(sf::Vector2f(650.0f, 400.0f), gameManager));
	mNPCManager.addNPC(std::make_unique<MiraStanton>(sf::Vector2f(1000.0f, 900.0f)));
	mNPCManager.addNPC(std::make_unique<ElliotMarlowe>(sf::Vector2f(100.0f, 0.0f)));
	mNPCManager.addNPC(std::make_unique<VincentHale>(sf::Vector2f(-100.0f, 0.0f)));
	mNPCManager.addNPC(std::make_unique<SeraphinaLumeris>(sf::Vector2f(-20.0f, 980.0f)));

	mAvailableTowers.push_back(2);
	mAvailableTowers.push_back(1);

	// Testing
	std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
	mInventory.addItem(std::move(woodItem), 1);
	
	std::unique_ptr<TowerBlueprint> towerB = std::make_unique<TowerBlueprint>();
	mInventory.addItem(std::move(towerB), 5);
    loadGame();
}

void RPGEngine::processEvents() {
	sf::Event event;
	while (mWindow.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			saveGame();
			mWindow.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::E) {
				if (!mShowDialogue) {
					mNPCManager.handleInteraction(mCharacter, mShowDialogue, mDialogueText);
					if (mShowDialogue) {
						mCurrentInteractingNPC = mNPCManager.getCurrentNPC();
						mNPCManager.interactWithCurrentNPC(mShowDialogue, mDialogueText);
					}
					else {
						for (auto& entity : mMap.getEntities()) {
							if (entity->isInteractable())
								if (entity->getInteractBounds().intersects(mCharacter.getBounds())) {
									entity->interact();
									break;
								}
						}
					}
				}
				else if (mCurrentInteractingNPC && mShowDialogue) {
					mNPCManager.interactWithCurrentNPC(mShowDialogue, mDialogueText);
				}
				else {
					mShowDialogue = false;
					if (mCurrentInteractingNPC) {
						mCurrentInteractingNPC->resumeMovement();
						mCurrentInteractingNPC->resetDialogue();
						mCurrentInteractingNPC = nullptr;
					}
				}
			}
			else if (event.key.code == sf::Keyboard::Escape) {
				if (mShowDialogue) {
					mShowDialogue = false;
					if (mCurrentInteractingNPC) {
						mCurrentInteractingNPC->resumeMovement();
						mCurrentInteractingNPC->resetDialogue();
						mCurrentInteractingNPC = nullptr;
					}
				}
				else if (mShowStartMenu) {
					mShowStartMenu = false;
				}
				else if (mShowBankMenu) {
					mShowBankMenu = false;
					mBankMenu.restart();
				}
				else if (mShowShopMenu) {
					mShowShopMenu = false;
				}
				else if (mShowAnalyzeMenu) {
					mShowAnalyzeMenu = false;
				}
				else {
					mShowMenu = !mShowMenu;
					if (!mShowMenu)
						mMenu.restart();
				}
			}
			else if (event.key.code == sf::Keyboard::O) {
				sf::String original = mDialogueText.getString();
				std::string text1 = original.toAnsiString();
				std::cout << text1 << std::endl;
				std::vector<sf::String> lines = wrapText(text1, mFont, 10, 425.0f);
				for (const auto& line : lines) {
					std::cout << line.toAnsiString() << std::endl;
				}
			}
			else if (event.key.code == sf::Keyboard::L) {
				mShowStartMenu = !mShowStartMenu;

				if (mShowStartMenu) {
					saveGame();
					mShowMenu = false;
					mShowBankMenu = false;
					mShowShopMenu = false;
					mShowAnalyzeMenu = false;
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
			else if (event.key.code == sf::Keyboard::B) {
				mShowBankMenu = !mShowBankMenu;

				if (mShowBankMenu) {
					mShowMenu = false;
					mShowStartMenu = false;
					mShowShopMenu = false;
					mShowAnalyzeMenu = false;
					if (mShowDialogue) {
						mShowDialogue = false;
						if (mCurrentInteractingNPC) {
							mCurrentInteractingNPC->resetDialogue();
							mCurrentInteractingNPC->resumeMovement();
							mCurrentInteractingNPC = nullptr;
						}
					}
				}
				else
					mBankMenu.restart();
			}
			else if (event.key.code == sf::Keyboard::K) {
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
				}
				else
					mShowShopMenu = false;
			}
			else if (event.key.code == sf::Keyboard::Q) {
				if (mShowMenu && mMenu.getMenuType() == "Inventory")
				{
					int slotIndex = mMenu.getInventoryMenu().getHoveredSlot();
					if (slotIndex != -1)
					{
						if (mInventory.getItemAt(slotIndex))
						{
							const Item* item = mInventory.getItemAt(slotIndex);
							const DroppedItem dropItem = DroppedItem(item, 
								{ mCharacter.getPosition().x + 10.f, mCharacter.getPosition().y - 10.f },
								mInventory.getItemQuantityAt(slotIndex));

							mInventory.removeItemAt(slotIndex);
							mDroppedItems.push_back(dropItem);
						}
					}
				}

				if (!mShowMenu && !mShowBankMenu && !mShowStartMenu && !mShowDialogue && !mShowAnalyzeMenu)
				{
					int slotIndex = mHotbar.getHoveredSlot();
					if (slotIndex != -1)
					{
						if (mInventory.getItemAt(slotIndex))
						{
							const Item* item = mInventory.getItemAt(slotIndex);
							const DroppedItem dropItem = DroppedItem(item, mCharacter.getPosition(),
								mInventory.getItemQuantityAt(slotIndex));

							mInventory.removeItemAt(slotIndex);
							mDroppedItems.push_back(dropItem);
						}
					}
				}
			}
			else if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Num3) {
				if (!mShowBankMenu && !mShowDialogue && !mShowMenu && !mShowStartMenu)
				{
					int slot = -1;
					if (event.key.code == sf::Keyboard::Num1)
						slot = 0;
					else if (event.key.code == sf::Keyboard::Num2)
						slot = 1;
					else if (event.key.code == sf::Keyboard::Num3)
						slot = 2;
					mHotbar.setHoveredSlot(slot);
				}
			}
			else if (event.key.code == sf::Keyboard::M) {
				mShowAnalyzeMenu = !mShowAnalyzeMenu;
				if (mShowAnalyzeMenu) {
					mShowMenu = false;
					mShowStartMenu = false;
					mShowShopMenu = false;
					mShowBankMenu = false;
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
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
				if (mShowMenu) {
					mMenu.handleMouseClick(mousePos);
				}
				if (mShowStartMenu) {
					mStartTowerDefenseMenu.handleMouseClick(mousePos);
				}
				if (mShowBankMenu) {
					mBankMenu.handleMouseClick(mousePos);
				}
				if (mShowShopMenu) {
					mShopMenu.handleMouseClick(mousePos);
				}
				if (mShowAnalyzeMenu) {
					mAnalyzeMenu.handleMouseClick(mousePos);
        }
        if(!mShowMenu && !mShowStartMenu && !mShowBankMenu && !mShowShopMenu && !mShowAnalyzeMenu && !mShowDialogue) {
          int slot = mHotbar.contains(mousePos);
					mHotbar.setHoveredSlot(slot);
				}
			}
		}
	}
}

void RPGEngine::update()
{
	float dt = mClock.restart().asSeconds();

	if (!mShowMenu) {
		if (!mShowStartMenu) {
			if (!mShowBankMenu) {
				if (!mShowShopMenu) {
					if (!mShowAnalyzeMenu) {
						sf::Vector2f previousPosition = mCharacter.getPosition();

						if (!mShowDialogue) {
							mHotbar.update();
						}

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
						mView.setCenter(mCharacter.getPosition());
						mFixedCamera.setCenter(mCameraFixedPosition);

						mTimeSystem.update(dt);

						if (!mNPCManager.playerClose(mCharacter.getPosition())) {
							mShowDialogue = false;
						}

						for (auto it = mDroppedItems.begin(); it != mDroppedItems.end();)
						{
							float distance = std::sqrt(
								std::pow(mCharacter.getPosition().x - it->getPosition().x, 2.f) +
								std::pow(mCharacter.getPosition().y - it->getPosition().y, 2.f)
							);

							if (it->getPickUpCap())
							{

								if (distance < 35.f)
								{
									const Item* item = it->getItem();
									if (item->getId() == 1)
										mInventory.addItem(std::make_unique<Wood>(), it->getQuantity());
									else if (item->getId() == 2)
										mInventory.addItem(std::make_unique<TowerBlueprint>(), it->getQuantity());

									it = mDroppedItems.erase(it);
									continue;
								}
							}
							else
							{
								if (distance > 35.f)
									it->setPickUpCap(true);
							}
							++it;
						}
						mMap.update();

						mShowInteract = false;

						for (auto& entity : mMap.getEntities())
						{
							if (!entity->isInteractable())	
								continue;

							if (entity->getInteractBounds().intersects(mCharacter.getBounds()))
							{
								mShowInteract = true;

								mInteractPos = entity->getInteractPosition();

								mInteractCircle.setPosition({mInteractPos.x + 
									entity->getInteractBounds().width + 1.5f, mInteractPos.y});
								
								mInteractText.setPosition({ mInteractCircle.getPosition().x + 5.9f,
									mInteractCircle.getPosition().y + 2.8f});

								break;
							}
						}

						if (!mShowInteract)
						{
							mInteractPos = { 0.f, 0.f };
							mInteractCircle.setPosition(mInteractPos);
							mInteractText.setPosition(mInteractPos);
						}
					
						mAnalyzeMenu.update();
					}
					else {
						sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
						mAnalyzeMenu.updateHover(mousePos);
					}
				}
				else {
					sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
					mShopMenu.updateHover(mousePos);
				}
			}
			else {
				sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
				mBankMenu.updateHover(mousePos);
				mBankMenu.update();
			}
		}
		else {
			sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
			mStartTowerDefenseMenu.updateHover(mousePos);
		}

	}
	else {
		sf::Vector2f mousePos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
		mMenu.updateHover(mousePos);
		mMenu.update(mCrystals, mInventory, mSkillTree);
	}
	mStartTowerDefenseMenu.update(mCrystals, mAvailableTowers);
}

void RPGEngine::render()
{
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

	if (!mShowBankMenu && !mShowMenu && !mShowStartMenu && !mShowShopMenu && !mShowAnalyzeMenu) {
		mWindow.setView(mWindow.getDefaultView());
		renderDateTime(mWindow, mFont, currentDate, currentTime);
		if(!mShowDialogue)
			mHotbar.render(mWindow);
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
	}

	if (mShowMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mMenu.render(mWindow);
	}

	if (mShowStartMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mStartTowerDefenseMenu.render(mWindow);
	}

	if (mShowBankMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mBankMenu.render(mWindow);
	}

	if (mShowShopMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mShopMenu.render(mWindow);
	}

	if (mShowAnalyzeMenu) {
		mWindow.setView(mWindow.getDefaultView());
		mAnalyzeMenu.render(mWindow);
	}

	mWindow.display();
}

void RPGEngine::renderDateTime(sf::RenderWindow& window, sf::Font& font, const std::string& date, const std::string& time)
{
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

void RPGEngine::resume(int crystals)
{
	mWindow.setView(mWindow.getDefaultView());
	loadGame();
	mCrystals = crystals;

	if (mShowDialogue) {
		mShowDialogue = false;
		mNPCManager.resumeCurrentNPC();
	}

	if (mShowMenu) {
		mShowMenu = false;
	}

	if (mShowStartMenu) {
		mShowStartMenu = false;
	}

	if (mShowBankMenu) {
		mShowBankMenu = false;
	}

	if (mShowShopMenu) {
		mShowShopMenu = false;
	}

	if (mShowAnalyzeMenu) {
		mShowAnalyzeMenu = false;
	}
}

void RPGEngine::saveGame() {
	std::vector<sf::Vector2f> npcPositions;
	std::vector<int> npcWaypoints;

	std::vector<int> inventoryItemId;
	std::vector<int> inventoryItemQuantity;
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
	
	for (auto& droppedItem : mDroppedItems) {
		droppedItemId.push_back(droppedItem.getItem()->getId());
		droppedItemXPos.push_back(droppedItem.getPosition().x);
		droppedItemYPos.push_back(droppedItem.getPosition().y);
		droppedItemQuantity.push_back(droppedItem.getQuantity());
	}


	/*for (int i = 0; i < inventoryItemId.size(); ++i)
		std::cout << inventoryItemId[i] << " " << inventoryItemQuantity[i] << std::endl;

	for (int i = 0; i < droppedItemId.size(); ++i)
		std::cout << droppedItemId[i] << " " << droppedItemXPos[i] << " " << droppedItemYPos[i] << " " << droppedItemQuantity[i] << std::endl;*/

	mSaveSystem.save(mCharacter.getPosition(), npcPositions, npcWaypoints, mCrystals,
		year, day, hour, minute, bankBalance, hasBorrowActive, penalty, interest, amountToRepay, daysToRepayment,
		startYear, startDay, startHour, startMinute, inventoryItemId, inventoryItemQuantity, droppedItemId, droppedItemXPos, droppedItemYPos,
		droppedItemQuantity, extracting, inSlot, completed, timerActive, startYear1, startDay1, startHour1, startMinute1, slotItemId,
		mIsInsideAStructure, mCameraFixedPosition);
}

void RPGEngine::loadGame() {
	sf::Vector2f playerPosition;
	std::vector<sf::Vector2f> npcPositions;
	std::vector<int> npcWaypoints;
	int crystals, year, day, hour, minute, bankBalance, penalty, interest, amountToRepay, daysToRepayment,
		startYear, startDay, startHour, startMinute, hasBorrowActive, extracting, inSlot, completed, timerActive,
		startYear1, startDay1, startHour1, startMinute1, slotItemId, insideStructure;
	std::vector<int> droppedItemId;
	std::vector<float> droppedItemXPos;
	std::vector<float> droppedItemYPos;
	std::vector<int> droppedItemQuantity;
	std::vector<int> inventoryItemId;
	std::vector<int> inventoryItemQuantity;

	if (mSaveSystem.load(playerPosition, npcPositions, npcWaypoints, crystals,
		year, day, hour, minute, bankBalance, hasBorrowActive, penalty, interest, amountToRepay, daysToRepayment,
        startYear, startDay, startHour, startMinute, inventoryItemId, inventoryItemQuantity, droppedItemId, 
		droppedItemXPos, droppedItemYPos, droppedItemQuantity, extracting, inSlot, completed, timerActive, startYear1,
		startDay1, startHour1, startMinute1, slotItemId, insideStructure, mCameraFixedPosition)) {

		mCharacter.setPosition(playerPosition);
		mIsInsideAStructure = insideStructure;
		mNPCManager.loadNPCStates(npcPositions, npcWaypoints);
		mCrystals = crystals;

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
			
			if (item != nullptr) {
				DroppedItem droppedItem(item, sf::Vector2f(droppedItemXPos[i], droppedItemYPos[i]), droppedItemQuantity[i]);
				mDroppedItems.push_back(droppedItem);
			}
			else
				std::cout << "Error : unknown drop item!" << std::endl;
		}

		for (size_t i = 0; i < inventoryItemId.size(); ++i) {
			if (inventoryItemId[i] == 1) {
				std::unique_ptr<Wood> woodItem = std::make_unique<Wood>();
				mInventory.addItem(std::move(woodItem), inventoryItemQuantity[i]);
			} else if (inventoryItemId[i] == 2) {
				std::unique_ptr<TowerBlueprint> towerBlueprint = std::make_unique<TowerBlueprint>();
				mInventory.addItem(std::move(towerBlueprint), inventoryItemQuantity[i]);
			}
		}
		
		for (int i = 0; i < mInventory.getSlotCount(); ++i) {
			if (mInventory.getItemAt(i))
				std::cout << mInventory.getItemAt(i)->getType() << " " << mInventory.getItemQuantityAt(i) << std::endl;
		}

		for (auto& droppedItem : mDroppedItems) {
			std::cout << droppedItem.getItem()->getType() << " " << droppedItem.getPosition().x << " "
				<< droppedItem.getPosition().y << " " << droppedItem.getQuantity() << std::endl;
		}
	} else {
		std::cerr << "Failed to load game data." << std::endl;
	}
}

void RPGEngine::resetSaveGame() {
	std::ofstream saveFile("savegame.txt", std::ofstream::trunc);

	if (saveFile.is_open())
		std::cout << "Save file cleared. Game will start from initial positions." << std::endl;
	else
		std::cerr << "Failed to clear save file." << std::endl;

	saveFile.close();

	mCharacter.setPosition(sf::Vector2f(400.f, 300.f));
	mNPCManager.loadNPCStates({}, {});
	mCrystals = 100;

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
    mShopMenu.regenerateIds();
	mIsInsideAStructure = false;
}
