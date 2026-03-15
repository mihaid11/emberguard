#include "DialogueActionExecutor.h"
#include "../gamengine/RPGEngine.h"
#include "../inventory/items/Item.h"
#include "../inventory/items/TowerBlueprint.h"
#include "../inventory/items/TowerBlueprintRare.h"
#include "../inventory/items/TowerBlueprintEpic.h"
#include "../inventory/items/TowerBlueprintMythic.h"
#include "../inventory/items/Wood.h"
#include <iostream>

DialogueActionExecutor::DialogueActionExecutor(StoryManager& storyManager, RPGEngine& rpgEngine, Inventory& inventory)
    : mStoryManager(storyManager), mRpgEngine(rpgEngine), mInventory(inventory) {

}

void DialogueActionExecutor::executeActions(const std::vector<DialogueAction>& actions) {
    for (const auto& action : actions)
        executeAction(action);
}

void DialogueActionExecutor::executeAction(const DialogueAction& action) {
    if (action.type == "set_story_flag") {
        mStoryManager.setFlag(action.target, action.value != 0);
    } else if (action.type == "set_menu_flag") {
        mRpgEngine.setFlag(action.target, action.value);
    } else if (action.type == "give_item") {
        std::unique_ptr<Item> mItem;
        if (action.target == "TowerBlueprint")
            mItem = std::make_unique<TowerBlueprint>();
        else if (action.target == "TowerBlueprintRare")
            mItem = std::make_unique<TowerBlueprintRare>();
        else if (action.target == "TowerBlueprintEpic")
            mItem = std::make_unique<TowerBlueprintEpic>();
        else if (action.target == "TowerBlueprintMythic")
            mItem = std::make_unique<TowerBlueprintMythic>();
        else if (action.target == "Wood")
            mItem = std::make_unique<Wood>();

        if (mItem)
            mInventory.addItem(std::move(mItem), action.value);
    } else if (action.type == "remove_item") {
        int itemId = -1;
        if (action.target == "TowerBlueprint")
            itemId = TowerBlueprint().getId();
        else if (action.target == "TowerBlueprintRare")
            itemId = TowerBlueprintRare().getId();
        else if (action.target == "TowerBlueprintEpic")
            itemId = TowerBlueprintEpic().getId();
        else if (action.target == "TowerBlueprintMythic")
            itemId = TowerBlueprintMythic().getId();
        else if (action.target == "Wood")
            itemId = Wood().getId();

        if (itemId != -1)
            mInventory.removeItemById(itemId, action.value);
    } else if (action.type == "change_crystals") {
        mRpgEngine.addCrystals(action.value);
    } else if (action.type == "set_chapter") {
        mStoryManager.setChapter(action.value);
    } else {
        std::cout << "Wrong action type!" << std::endl;
    }
}

