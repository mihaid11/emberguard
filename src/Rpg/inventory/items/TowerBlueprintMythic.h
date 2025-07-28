#pragma once
#include "Item.h"

class TowerBlueprintMythic : public Item {
public:
    TowerBlueprintMythic();
    std::string getType() const override;
    std::unique_ptr<Item> clone() const override {
        return std::make_unique<TowerBlueprintMythic>(*this);
    };
};

