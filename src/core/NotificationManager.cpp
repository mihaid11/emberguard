#include "NotificationManager.h"
#include <cinttypes>
#include <iostream>

NotificationManager::NotificationManager() : mSpawnTimer(0.f) {
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

    mPendingNotifications.push(notification);
}

void NotificationManager::render(sf::RenderWindow& window) {
    for (const auto& notification: mNotifications) {
        window.draw(notification.box);
        window.draw(notification.header);
        window.draw(notification.title);
    }
}

void NotificationManager::update(float dt) {
    float startX = 20.f;
    float startY = 20.f;
    float gap = 15.f;

    if (mSpawnTimer > 0.f)
        mSpawnTimer -= dt;

    if (mSpawnTimer <= 0.f && !mPendingNotifications.empty()) {
        Notification notification = mPendingNotifications.front();
        mPendingNotifications.pop();

        if (!mNotifications.empty())
            notification.currentY = mNotifications.back().currentY - 10.f;
        else
            notification.currentY = startY - 70.f;

        mNotifications.push_back(notification);
        mSpawnTimer = 0.8f;
    }

    for (auto it = mNotifications.begin(); it != mNotifications.end();) {
        it->lifeTime -= dt;

        if (it->lifeTime <= 0.f)
            it = mNotifications.erase(it);
        else
            ++it;
    }

    for (int i = 0; i < mNotifications.size(); ++i) {
        auto& notification = mNotifications[i];

        float targetY = startY + i * (notification.box.getSize().y + gap);

        if (notification.lifeTime < 1.f)
            targetY -= (1.f - notification.lifeTime) * notification.box.getSize().y;

        notification.currentY += (targetY - notification.currentY) * 3.5f * dt;

        float height = notification.title.getLocalBounds().height + notification.header.getLocalBounds().height;
        float textsGap = (notification.box.getSize().y - height) / 3.f;

        notification.box.setPosition(startX, notification.currentY);
        notification.header.setPosition(startX + 10.f, notification.currentY + textsGap);
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
