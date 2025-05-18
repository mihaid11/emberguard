#pragma once
#include "Item.h"

class Wood : public Item {
public:
    Wood();
    std::string getType() const override;
};
