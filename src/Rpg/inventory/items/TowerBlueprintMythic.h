#pragma once
#include "Item.h"

class TowerBlueprintMythic : public Item {
public:
	TowerBlueprintMythic();
	std::string getType() const override;
};

