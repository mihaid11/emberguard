#include "Inventory.h"

Inventory::Inventory(int rows, int cols) : mRows(rows), mCols(cols) {
    mSlots.resize(rows * cols);
}

void Inventory::addItem(std::unique_ptr<Item> item, int quantity) {
    int slotWithItem = findSlotWithItem(item->getId());

    if (slotWithItem != -1) {
        mSlots[slotWithItem].quantity += quantity;
    } else {
        int emptySlot = getFirstEmptySlot();
        if (emptySlot != -1)
            mSlots[emptySlot] = { std::move(item), quantity };
    }
}

void Inventory::swapItems(int slot1, int slot2) {
    if (slot1 >= 0 && slot1 < mSlots.size() && slot2 >= 0 && slot2 < mSlots.size())
        std::swap(mSlots[slot1], mSlots[slot2]);
}

const Item* Inventory::getItemAt(int slot) const {
    if (slot >= 0 && slot < mSlots.size())
        return mSlots[slot].item.get();
    return nullptr;
}

int Inventory::getItemQuantityAt(int slot) const {
    if (slot >= 0 && slot < mSlots.size())
        return mSlots[slot].quantity;
    return 0;
}

void Inventory::setItemQuantityAt(int slot, int quantity) {
    if (slot >= 0 && slot < mSlots.size())
        mSlots[slot].quantity = quantity;
}

int Inventory::getSlotCount() const {
    return mRows * mCols;
}

void Inventory::resize(int newRows, int newCols) {
    mRows = newRows;
    mCols = newCols;
    mSlots.resize(newRows * newCols);
}

void Inventory::removeItemAt(int slotIndex) {
    mSlots[slotIndex].item = nullptr;
    mSlots[slotIndex].quantity = 0;
}

int Inventory::getFirstEmptySlot() const {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (!mSlots[i].item)
            return i;
    }
    return -1;
}

int Inventory::findSlotWithItem(int id) const {
    for (int i = 0; i < mSlots.size(); ++i) {
        if (mSlots[i].item && mSlots[i].item->getId() == id)
            return i;
    }
    return -1;
}

