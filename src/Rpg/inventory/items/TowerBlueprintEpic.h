#pragma once
#include "Item.h"

class TowerBlueprintEpic : public Item {
public:
    TowerBlueprintEpic();
    std::string getType() const override;
    std::unique_ptr<Item> clone() const override {
        return std::make_unique<TowerBlueprintEpic>(*this);
    };
};

