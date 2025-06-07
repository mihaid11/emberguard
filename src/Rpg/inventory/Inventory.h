#pragma once
#include <vector>
#include <memory>
#include "items/Item.h"

struct InventoryItem {
    std::shared_ptr<Item> item;
    int quantity;
};

class Inventory {
public:
    Inventory(int rows = 2, int cols = 3);

    void addItem(std::unique_ptr<Item> item, int quantity);
    void swapItems(int slot1, int slot2);
    int getItemQuantityAt(int slot) const;
    void setItemQuantityAt(int slot, int quantity);
    const Item* getItemAt(int slot) const;
    int getSlotCount() const;
    void resize(int newRows, int newCols);
    void removeItemAt(int slotIndex);

private:
    std::vector<InventoryItem> mSlots;
    int mRows;
    int mCols;

    int getFirstEmptySlot() const;
    int findSlotWithItem(int id) const;
};

