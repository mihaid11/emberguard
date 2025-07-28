#pragma once
#include "Item.h"

class Wood : public Item {
public:
    Wood();
    std::string getType() const override;
    std::unique_ptr<Item> clone() const override {
        return std::make_unique<Wood>(*this);
    };
};

