#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../enemy/Enemy.h"
#include "PathsConfig.h"
#include "../enemy/WastelandMarauder.h"
#include "../enemy/RadiationBerserker.h"
#include "../enemy/EmpireScout.h"

using Path = std::vector<sf::Vector2f>;

class Wave {
public:
    struct EnemyTypeInfo {
        std::string type;
        int count;
        float spawnInterval;
        int pathNumber;
    };

    Wave(int level, int waveNumber, const std::vector<Path>& paths,
         const std::vector<EnemyTypeInfo>& enemyTypes);

    void update(float dt, std::vector<Enemy>& enemies);
    bool isComplete() const;
    int getEnemyCount() const;

private:
    int mTotalEnemies;
    int mEnemiesSpawned;
    float mGlobalSpawnTimer;
    std::vector<Path> mPaths;
    std::vector<EnemyTypeInfo> mEnemyTypes;
    std::vector<int> mEnemiesSpawnedPerType;

    Enemy createEnemy(const std::string& type, const Path& path) const;
};

