#pragma once
#include "Item.h"

class TowerBlueprintEpic : public Item {
public:
	TowerBlueprintEpic();
	std::string getType() const override;
};

