#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"
#include "../../entities/DrawableEntity.h"

class DroppedItem : public DrawableEntity {
public:
    DroppedItem(const Item* item, const sf::Vector2f& position, int quantity);

    void render(sf::RenderWindow& window) override;
    bool isPickedUp(const sf::FloatRect& playerBounds) const;

    int getQuantity() const;
    const Item* getItem();
    sf::Vector2f getPosition() const override;
    float getHeight() const override;
    sf::CircleShape getItemShape() const { return mItemShape; }
    bool getPickUpCap();
    void setPickUpCap(bool can);

private:
    const Item* mItem;
    sf::CircleShape mItemShape;
    sf::Vector2f mPosition;
    int mQuantity;
    bool mCanPickUp;
};

