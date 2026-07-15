#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "../src/Rpg/dialogueSystem/DialogueManager.h"
#include "../src/Rpg/dialogueSystem/DialogueDatabase.h"
#include "../src/Rpg/story/StoryManager.h"

TEST_CASE("Dialogue system logic", "dialogue") {
    DialogueDatabase db;
    StoryManager storyManager;
    storyManager.setChapter(1);

    SECTION("Action correctness") {
        bool isLoaded = db.loadDialogueFromFile("../tests/test_dialogue.json");
        REQUIRE(isLoaded == true);

        auto optDialogue = db.getDialogueForNPC("grand_master", storyManager);
        REQUIRE(optDialogue.has_value() == true);

        DialogueManager manager;
        manager.startDialogue(optDialogue.value());

        REQUIRE(manager.getCurrentDialogueText() == "Halt! Who goes there?");
        manager.getCurrentDialogue().indexIncrement();
        REQUIRE(manager.currentHasChoices() == true);
        REQUIRE(manager.getChoices().size() == 3);

        manager.choose(0);
        REQUIRE(manager.getCurrentDialogueText() == "A friend? Prove it.");
        
        manager.getCurrentDialogue().indexIncrement();
        REQUIRE(manager.getCurrentDialogueText() == "What is the secret password?");
        REQUIRE(manager.currentHasChoices() == true);
        REQUIRE(manager.getChoices().size() == 2);

        auto actions = manager.chooseWithActions(0);
        REQUIRE(manager.getCurrentDialogueText() == "Correct. The ancient texts spoke of you.");
        
        REQUIRE(actions.size() == 2);
        REQUIRE(actions[0].type == "give_item");
        REQUIRE(actions[1].type == "set_story_flag");
    }

    SECTION("Json loading performance") {
        BENCHMARK("Load and parse dialogue JSON") {
            DialogueDatabase benchDb;
            return benchDb.loadDialogueFromFile("../tests/test_dialogue.json");
        };
    }
    
    SECTION("Performance test") {
        db.loadDialogueFromFile("../tests/test_dialogue.json");
        
        BENCHMARK("Resolve conditions") {
            return db.getDialogueForNPC("grand_master", storyManager).has_value();
        };
    }
}
