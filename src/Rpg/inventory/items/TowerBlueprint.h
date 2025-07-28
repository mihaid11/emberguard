#pragma once
#include "Item.h"

class TowerBlueprint : public Item {
public:
    TowerBlueprint();
    std::string getType() const override;
    std::unique_ptr<Item> clone() const override {
        return std::make_unique<TowerBlueprint>(*this);
    };
};

