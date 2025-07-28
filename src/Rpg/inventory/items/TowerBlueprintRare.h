#pragma once
#include "Item.h"

class TowerBlueprintRare : public Item {
public:
    TowerBlueprintRare();
    std::string getType() const override;
    std::unique_ptr<Item> clone() const override {
        return std::make_unique<TowerBlueprintRare>(*this);
    };
};

