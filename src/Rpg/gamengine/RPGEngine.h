#pragma once
#include <SFML/Graphics.hpp>
#include <string.h>
#include "../mainCharacter/MainCharacter.h"
#include "../npcs/NPC.h"
#include "../npcs/NPCManager.h"
#include "../dialogueSystem/DialogueManager.h"
#include "../story/StoryManager.h"
#include "../menues/RpgMenu.h"
#include "../saveSystem/SaveSystem.h"
#include "../menues/StartTowerDefenseMenu.h"
#include "../npcs/MiraStanton.h"
#include "../npcs/ElliotMarlowe.h"
#include "../npcs/GarrickStone.h"
#include "../npcs/VincentHale.h"
#include "../npcs/SeraphinaLumeris.h"
#include "../../core/TimeSystem.h"
#include "../menues/BankMenu.h"
#include "../inventory/items/Wood.h"
#include "../inventory/items/TowerBlueprint.h"
#include "../inventory/items/TowerBlueprintRare.h"
#include "../inventory/items/TowerBlueprintEpic.h"
#include "../inventory/items/TowerBlueprintMythic.h"
#include "../inventory/items/DroppedItem.h"
#include "../inventory/Hotbar.h"
#include "../menues/ShopMenu.h"
#include "../menues/AnalyzeMenu.h"
#include "../menues/ChestMenu.h"
#include "../../TowerDefense/menues/LevelCompleteMenu.h"
#include "../map/entities/DrawableEntity.h"
#include "../map/buildings/MCHouse.h"
#include "../map/buildings/MCHouseInt.h"
#include "../map/buildings/Barrier.h"
#include "../map/buildings/Bed.h"
#include "../map/buildings/Chest.h"
#include "../../core/TransitionSystem.h"
#include "../map/entities/EntityFactory.h"
#include "../map/zones/ZoneManager.h"
#include "../map/entities/WaypointManager.h"

class GameManager;

class RPGEngine {
public:
    RPGEngine(sf::RenderWindow& window, GameManager* gameManager);

    void processEvents();
    void update();
    void render();
    void renderDialogueChoices();

    void resume(int crystals);
    void closeMenues();

    void saveGame();
    void loadGame();
    void resetSaveGame();
    void resetToDefault();
    bool saveExists(int saveNumber) const;
    bool deleteSave(int saveNumber);
    void newGame();
    void setSaveNumber(int saveNumber);
    void setFlag(std::string name, bool value);
    void addCrystals(int value);
    void setCrystals(int crystals);
    void addXp(int xp);
    void advanceTowerDefenseLevel();

    void initialize();
    void uninitialize();
    void enterRPG();
    void exitRPG();

    SaveSystem& getSaveSystem();
    Inventory& getInventory();
    MainCharacter& getPlayer();

private:
    sf::RenderWindow& mWindow;
    MainCharacter mCharacter;

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
    bool mShowChestMenu;
    sf::Text mDialogueText;
    sf::Font mFont;

    GameManager* mGameManager;
    int mCrystals;
    int mCurrentTowerDefenseLevel;
    std::vector<int> mAvailableTowers;
    int mChapter;

    SaveSystem mSaveSystem;
    int mSaveNumber;
    std::string mSavePath;

    std::vector<DrawableEntity*> mRenderQueue;
    sf::RectangleShape mDialogueBox;
    sf::RectangleShape mSeparationLine;
    float mDialogueTextWidth;
    float mDialogueLineHeight;
    sf::Sprite mIconSprite;

    sf::RectangleShape mDateTimeBackground;
    sf::Text mDateText;
    sf::Text mTimeText;

    StoryManager mStoryManager;
    DialogueDatabase mDialogueDatabase;
    NPCManager mNPCManager;
    //DialogueManager mDialogueManager;
    SkillTree mSkillTree;
    Inventory mInventory;
    Inventory mChestInventory;
    Hotbar mHotbar;
    RpgMenu mMenu;
    ShopMenu mShopMenu;
    StartTowerDefenseMenu mStartTowerDefenseMenu;
    BankMenu mBankMenu;
    AnalyzeMenu mAnalyzeMenu;
    ChestMenu mChestMenu;
    LevelCompleteMenu mLevelCompleteMenu;

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
    bool mPaused;
    bool mIsInitialized;

    sf::CircleShape mDialogueInteractCircle;
    sf::Text mDialogueInteractText;

    sf::Vector2f mLastMousePos;
    int mSelectedChoice;
    std::vector<sf::RectangleShape> mChoiceBoxes;
    std::vector<sf::Text> mChoiceTexts;
    void updateDialogueChoices();

    bool mIsInsideAStructure;
    int mStructureIndex;
    sf::Vector2f mCameraFixedPosition;
    sf::View mFixedCamera;

    TransitionSystem mTransitionSystem;

    GameContext mGameContext;
    ZoneManager mZoneManager;
    WaypointManager mWaypointManager;

    sf::Vector2f calculateDropPosition();
};

