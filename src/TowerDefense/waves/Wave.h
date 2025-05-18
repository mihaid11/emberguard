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

class Wave
{
public:
    Wave(int level, int waveNumber, const std::vector<int>& pathIndices);

    void update(float dt, std::vector<Enemy>& enemies);
    bool isComplete() const;

    int getEnemyCount() const;

private:
    struct EnemyInfo {
        int count;
        std::function<Enemy(const Path&)> createEnemy;
    };

    int mTotalEnemies;
    int mEnemiesSpawned;
    float mSpawnTimer;
    std::vector<Path> mPaths;
    std::vector<EnemyInfo> mEnemyTypes;
    int mCurrentEnemyType;

    void setupWave(int level, int waveNumber);
};