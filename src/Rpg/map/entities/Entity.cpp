#include "Entity.h"
#include <iostream>

Entity::Entity(const sf::Vector2f& position, const sf::Vector2f& size,
               const std::string filename, const sf::Vector2f& collPosition,
               const sf::Vector2f& collSize, int canInteract,
               const sf::Vector2f& interactPos, const sf::Vector2f& interactSize)
    : mCanInteract(canInteract) {
    // If filename is empty than the entity is not drawable => dont load texture from file
    if(filename != "") {
        sf::Texture& texture = ResourceManager::getInstance().getTexture(filename);
        mSprite.setTexture(texture);

        if (size.x > 0.f && size.y > 0.f) {
            float scaleX = size.x / static_cast<float>(texture.getSize().x);
            float scaleY = size.y / static_cast<float>(texture.getSize().y);
            mSprite.setScale(scaleX, scaleY);
        }
    }
    mSprite.setPosition(position);

    mCollisionZone.setSize(collSize);
    mCollisionZone.setPosition(collPosition);
    mCollisionZone.setFillColor(sf::Color::Red);

    mInteractableZone.setSize(interactSize);
    mInteractableZone.setPosition(interactPos);
    mInteractableZone.setFillColor(sf::Color::Blue);
}

void Entity::render(sf::RenderWindow& window) {
    window.draw(mSprite);
    //window.draw(mCollisionZone);
    //window.draw(mInteractableZone);
}

void Entity::update() {

}

sf::FloatRect Entity::getBounds() const {
    return mSprite.getGlobalBounds();
}

sf::FloatRect Entity::getCollisionBounds() const {
    return mCollisionZone.getGlobalBounds();
}

sf::Vector2f Entity::getPosition() const {
    return mSprite.getPosition();
}

sf::Vector2f Entity::getInteractPosition() const {
    return mInteractableZone.getPosition();
}

float Entity::getHeight() const {
    return mSprite.getGlobalBounds().height;
}

int Entity::isInteractable() const {
    return mCanInteract;
}

sf::FloatRect Entity::getInteractBounds() const {
    return mInteractableZone.getGlobalBounds();
}

void Entity::setAlpha(int alpha) {
    sf::Color entityColor = mSprite.getColor();
    entityColor.a = alpha;
    mSprite.setColor(entityColor);
}

void Entity::interact() {
    std::cout << "Interacting!" << std::endl;
}
