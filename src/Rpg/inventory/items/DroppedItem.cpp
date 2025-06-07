#include "DroppedItem.h"

DroppedItem::DroppedItem(const Item* item, const sf::Vector2f& position, int quantity)
    : mItem(item), mPosition(position), mQuantity(quantity), mCanPickUp(false) {
    mItemShape.setRadius(11.5f);
    mItemShape.setFillColor(sf::Color(100, 100, 100, 50));
    mItemShape.setOutlineColor(sf::Color::White);
    mItemShape.setOutlineThickness(1.5f);
    mItemShape.setPosition(mPosition);
}

void DroppedItem::render(sf::RenderWindow& window) {
    window.draw(mItemShape);

    //Draw the icon
    // TODO : Implement item icon drawing
    //sf::RectangleShape icon = mItem->getIcon();
    //icon.setScale(sf::Vector2f(0.35f, 0.35f));
    //icon.setPosition(mPosition.x + mItemShape.getRadius() - icon.getSize().x / 2.f, mPosition.y + mItemShape.getRadius() - icon.getSize().y / 2.f);
    //window.draw(icon);
}

bool DroppedItem::isPickedUp(const sf::FloatRect& playerBounds) const {
    return mItemShape.getGlobalBounds().intersects(playerBounds);
}

int DroppedItem::getQuantity() const {
    return mQuantity;
}

const Item* DroppedItem::getItem() {
    return mItem;
}

sf::Vector2f DroppedItem::getPosition() const {
    return mPosition;
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

