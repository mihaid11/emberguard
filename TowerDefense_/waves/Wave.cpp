#include "Wave.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

Wave::Wave(int level, int waveNumber, const std::vector<int>& pathIndices)
    : mTotalEnemies(0),
    mEnemiesSpawned(0),
    mSpawnTimer(0.0f),
    mCurrentEnemyType(0)
{
    std::cout << "Creating wave " << waveNumber << " for level " << level << std::endl;

    // Setup paths for the wave
    if (level > 0 && level <= Paths.size()) {
        const auto& levelPaths = Paths[level - 1];
        for (int index : pathIndices) {
            if (index >= 0 && index < levelPaths.size()) {
                mPaths.push_back(levelPaths[index]);
            }
        }
    }

    setupWave(level, waveNumber);

    for (const auto& enemyInfo : mEnemyTypes) {
        mTotalEnemies += enemyInfo.count;
    }
}

void Wave::setupWave(int level, int waveNumber) {
    if (level == 1) {
        if (waveNumber == 1)
            mEnemyTypes.push_back({ 15, [](const Path& path) { return WastelandMarauder(path); } });
        if (waveNumber == 2)
            mEnemyTypes.push_back({ 10, [](const Path& path) { return RadiationBerserker(path); } });
        if (waveNumber == 3)
            mEnemyTypes.push_back({ 35, [](const Path& path) { return EmpireScout(path); } });
    }
}

void Wave::update(float dt, std::vector<Enemy>& enemies)
{
    mSpawnTimer += dt;

    while (mCurrentEnemyType < mEnemyTypes.size() &&
        mSpawnTimer >= mEnemyTypes[mCurrentEnemyType].createEnemy(mPaths[0]).getSpawnTime() &&
        mEnemiesSpawned < mTotalEnemies)
    {
        mSpawnTimer = 0.0f;
        if (mEnemiesSpawned < mEnemyTypes[mCurrentEnemyType].count)
        {
            const Path& path = mPaths[rand() % mPaths.size()];
            enemies.push_back(mEnemyTypes[mCurrentEnemyType].createEnemy(path));
            mEnemiesSpawned++;
        }
        else
        {
            mCurrentEnemyType++;
        }
    }
}

bool Wave::isComplete() const
{
    return mEnemiesSpawned >= mTotalEnemies;
}

int Wave::getEnemyCount() const
{
    return mTotalEnemies;
}

