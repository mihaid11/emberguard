#include "DroppedItem.h"

DroppedItem::DroppedItem(std::shared_ptr<const Item> item, const sf::Vector2f& position, int quantity)
    : mItem(std::move(item)), mPosition(position), mQuantity(quantity), mCanPickUp(false) {
    mItemShape.setRadius(8.f);
    mItemShape.setFillColor(sf::Color(20, 20, 20, 100));
    mItemShape.setOutlineColor(sf::Color::White);
    mItemShape.setOutlineThickness(1.5f);
    mItemShape.setPosition(mPosition);
}

void DroppedItem::render(sf::RenderWindow& window) {
    window.draw(mItemShape);

    if (mItem) {
        sf::RectangleShape icon = mItem->getIcon();
        icon.setScale(0.2f, 0.2f);

        sf::Vector2f iconSize = icon.getSize();
        iconSize.x *= icon.getScale().x;
        iconSize.y *= icon.getScale().y;

        sf::Vector2f iconPos(
            mItemShape.getPosition().x + mItemShape.getRadius() - iconSize.x / 2.f,
            mItemShape.getPosition().y + mItemShape.getRadius() - iconSize.y / 2.f
        );

        icon.setPosition(iconPos);
        window.draw(icon);
    }
}

bool DroppedItem::isPickedUp(const sf::FloatRect& playerBounds) const {
    return mItemShape.getGlobalBounds().intersects(playerBounds);
}

int DroppedItem::getQuantity() const {
    return mQuantity;
}

const Item* DroppedItem::getItem() {
    return mItem.get();
}

sf::Vector2f DroppedItem::getPosition() const {
    return mPosition;
}

sf::Vector2f DroppedItem::getCenterPosition() const {
    return mPosition + sf::Vector2f(mItemShape.getRadius(), mItemShape.getRadius());
}

float DroppedItem::getHeight() const {
    return mItemShape.getGlobalBounds().height;
}

bool DroppedItem::getPickUpCap() {
    return mCanPickUp;
}

void DroppedItem::setPickUpCap(bool can) {
    mCanPickUp = can;
}

