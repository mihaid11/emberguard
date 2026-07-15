#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "../src/Rpg/inventory/Inventory.h"
#include "../src/Rpg/inventory/items/Wood.h"
#include <memory>

TEST_CASE("Inventory test", "inventory") {
    Inventory inventory;

    SECTION("Adding an item") {
        inventory.addItem(std::make_unique<Wood>(), 5);

        REQUIRE(inventory.getItemQuantityAt(0) == 5);

        REQUIRE(inventory.getItemAt(0) != nullptr);
        REQUIRE(inventory.getItemAt(0)->getId() == 1);
    }

    SECTION("Clearing and reloading inventory") {
        inventory.addItem(std::make_unique<Wood>(), 5);
        REQUIRE(inventory.getItemQuantityAt(0) == 5);

        inventory.clear();
        REQUIRE(inventory.getItemQuantityAt(0) == 0);
        REQUIRE(inventory.getItemAt(0) == nullptr);

        inventory.addItem(std::make_unique<Wood>(), 2);
        REQUIRE(inventory.getItemQuantityAt(0) == 2);
    }

    SECTION("Benchmark") {
        BENCHMARK("Adding 1000 items") {
            Inventory inventory;
            
            for (int i = 0; i < 1000; ++i)
                inventory.addItem(std::make_unique<Wood>(), 1);

            return inventory.getItemQuantityAt(0);
        };
    }
}
