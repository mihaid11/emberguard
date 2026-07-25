#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

class NotificationManager {
public:
    NotificationManager();

    enum class Type {
        QuestStarted,
        QuestCompleted
    };

    void addNotification(Type type, const std::string& title);
    void render(sf::RenderWindow& window);
    void update(float dt);

private:
    sf::Font mFont;

    struct Notification {
        sf::RectangleShape box;
        sf::Text header;
        sf::Text title;
        float lifeTime;
        float maxLifeTime;

        float currentY;
    };

    std::vector<Notification> mNotifications;
    std::queue<Notification> mPendingNotifications;
    float mSpawnTimer;
};
