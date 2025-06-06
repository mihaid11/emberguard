#pragma once
#include "Item.h"

class TowerBlueprintRare : public Item {
public:
	TowerBlueprintRare();
	std::string getType() const override;
};

