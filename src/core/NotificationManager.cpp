#include "NotificationManager.h"
#include <cinttypes>
#include <iostream>

NotificationManager::NotificationManager() {
    if (!mFont.loadFromFile("assets/fonts/gameFont.ttf"))
        std::cout << "Failed to load font in NotificationManager" << std::endl;
}

void NotificationManager::addNotification(Type type, const std::string& title) {
    Notification notification;
    notification.lifeTime = 5.f;
    notification.maxLifeTime = 5.f;

    notification.box.setSize(sf::Vector2f(200.f, 70.f));
    notification.box.setFillColor(sf::Color(30, 30, 30, 230));
    notification.box.setOutlineThickness(2.f);

    notification.header.setFont(mFont);
    notification.header.setCharacterSize(14);
    notification.header.setFillColor(sf::Color::White);

    if (type == Type::QuestStarted)
        notification.header.setString("QUEST STARTED");
    else if (type == Type::QuestCompleted)
        notification.header.setString("QUEST COMPLETED");

    notification.title.setFont(mFont);
    notification.title.setString(title);
    notification.title.setCharacterSize(14);
    notification.title.setFillColor(sf::Color::White);

    mNotifications.push_back(notification);
}

void NotificationManager::render(sf::RenderWindow& window) {
    for (const auto& notification: mNotifications) {
        window.draw(notification.box);
        window.draw(notification.header);
        window.draw(notification.title);
    }
}

void NotificationManager::update(float dt) {
    for (auto it = mNotifications.begin(); it != mNotifications.end();) {
        it->lifeTime -= dt;

        if (it->lifeTime <= 0.f)
            it = mNotifications.erase(it);
        else
            ++it;
    }

    float startX = 20.f;
    float startY = 20.f;
    float gap = 15.f;

    for (int i = 0; i < mNotifications.size(); ++i) {
        auto& notification = mNotifications[i];

        float targetY = startY + i * (notification.box.getSize().y + gap);
        float currentY = targetY;
        float timeAlive = notification.maxLifeTime - notification.lifeTime;
        float slideDuration = 0.75f;

        if (timeAlive < slideDuration) {
            float progress = timeAlive / slideDuration;
            float offset = 60.f;

            currentY = targetY - offset * (1.f - progress) * (1.f - progress);
        }

        float height = notification.title.getLocalBounds().height + notification.header.getLocalBounds().height;
        float textsGap = (notification.box.getSize().y - height) / 3.f;

        notification.box.setPosition(startX, currentY);
        notification.header.setPosition(startX + 10.f, currentY + textsGap);
        notification.title.setPosition(startX + 10.f, notification.header.getPosition().y + notification.header.getLocalBounds().height + textsGap);

        if (notification.lifeTime < 1.f) {
            sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * notification.lifeTime);
            notification.box.setFillColor(sf::Color(notification.box.getFillColor().r, notification.box.getFillColor().g,
                                                    notification.box.getFillColor().b, alpha));
            notification.header.setFillColor(sf::Color(notification.header.getFillColor().r, notification.header.getFillColor().g,
                                                       notification.header.getFillColor().b, alpha));
            notification.title.setFillColor(sf::Color(notification.title.getFillColor().r, notification.title.getFillColor().g,
                                                      notification.title.getFillColor().b, alpha));
        }
    }
}
