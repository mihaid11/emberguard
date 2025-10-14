#pragma once
#include "items/Item.h"
#include <memory>
#include <unordered_map>
#include <functional>

class ItemFactory {
public:
    static std::unique_ptr<Item> createItemById(int id) {
        switch (id) {
            case 1: return std::make_unique<Wood>();
            case 2: return std::make_unique<TowerBlueprint>();
            case 3: return std::make_unique<TowerBlueprintRare>();
            case 4: return std::make_unique<TowerBlueprintEpic>();
            case 5: return std::make_unique<TowerBlueprintMythic>();
            default: return nullptr;
        }
    }
};

