#pragma once
#include <SFML/Graphics.hpp>
#include "../mainCharacter/MainCharacter.h"
#include "../map/GameMap.h"
#include "../npcs/NPC.h"
#include "../npcs/NPCManager.h"
#include "../dialogueSystem/DialogueManager.h"
#include "../menues/Menu.h"
#include "../saveSystem/SaveSystem.h"		
#include "../menues/StartTowerDefenseMenu.h"
#include "../npcs/MiraStanton.h"
#include "../npcs/ElliotMarlowe.h"
#include "../npcs/GarrickStone.h"
#include "../npcs/VincentHale.h"
#include "../npcs/SeraphinaLumeris.h"
#include "../../TimeSystem.h"
#include "../menues/BankMenu.h"
#include "../inventory/items/Wood.h"
#include "../inventory/items/TowerBlueprint.h"
#include "../inventory/items/DroppedItem.h"
#include "../inventory/Hotbar.h"
#include "../menues/ShopMenu.h"
#include "../menues/AnalyzeMenu.h"
#include "../entities/DrawableEntity.h"
#include "../map/buildings/MCHouse.h"
#include "../map/buildings/MCHouseInt.h"
#include "../map/buildings/Barrier.h"
#include "../map/buildings/Bed.h"
//#include "../map/zones/ZoneManager.h"

class GameManager;

class RPGEngine
{
public:
	RPGEngine(sf::RenderWindow& window, GameManager* gameManager);

	void processEvents();
	void update();
	void render();
	void renderDateTime(sf::RenderWindow& window, sf::Font& font, const std::string& date, const std::string& time);
	void resume(int crystals);

	void saveGame();
	void loadGame();
	void resetSaveGame();

private:
	sf::RenderWindow& mWindow;
	MainCharacter mCharacter;
	GameMap mMap;

	//NPCs
	VincentHale mVincentHale;
	GarrickStone mGarrickStone;
	MiraStanton mMiraStanton;
	ElliotMarlowe mElliotMarlowe;
	SeraphinaLumeris mSeraphinaLumeris;

	sf::Clock mClock;
	sf::View mView;

	bool mShowMenu;
	bool mShowDialogue;
	bool mShowStartMenu;
	bool mShowShopMenu;
	bool mShowBankMenu;
	bool mShowAnalyzeMenu;
	sf::Text mDialogueText;
	sf::Font mFont;

	GameManager* mGameManager;
	int mCrystals;
	int mCurrentLevel;
	std::vector<int> mAvailableTowers;

	SaveSystem mSaveSystem;
	NPCManager mNPCManager;
	//DialogueManager mDialogueManager;
	SkillTree mSkillTree;
	Inventory mInventory;
	Hotbar mHotbar;
	Menu mMenu;
	ShopMenu mShopMenu;
	StartTowerDefenseMenu mStartTowerDefenseMenu;
	BankMenu mBankMenu;
	AnalyzeMenu mAnalyzeMenu;

	NPC* mCurrentInteractingNPC;
	TimeSystem mTimeSystem;

	int mStorageCapacity;
	std::vector<DroppedItem> mDroppedItems;

	sf::RectangleShape mBorderUp;
	sf::RectangleShape mBorderDown;
	sf::RectangleShape mBorderLeft;
	sf::RectangleShape mBorderRight;

	sf::CircleShape mInteractCircle;
	sf::Text mInteractText;
	bool mShowInteract;
	sf::Vector2f mInteractPos;

	sf::CircleShape mDialogueInteractCircle;
	sf::Text mDialogueInteractText;

	bool mIsInsideAStructure;
	sf::Vector2f mCameraFixedPosition;
	sf::View mFixedCamera;
	
	// TODO : Implement method for efficient tile rendering
	//ZoneManager mZoneManager;
};

