    #include "Item.h"

    Item::Item(const std::string& name, const std::string& description, int id, int price, const sf::RectangleShape& icon)
        : mName(name), mDescription(description), mId(id), mPrice(price), mIcon(icon) {}

    const std::string& Item::getName() const {
        return mName;
    }

    const std::string& Item::getDescription() const {
        return mDescription;
    }

    int Item::getId() const {
        return mId;
    }

    int Item::getPrice() const {
        return mPrice;
    }

    const sf::RectangleShape& Item::getIcon() const {
        return mIcon;
    }
