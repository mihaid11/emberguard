#pragma once
#include "Item.h"

class TowerBlueprint : public Item {
public:
    TowerBlueprint();
    std::string getType() const override;
};

