#include "WaveManager.h"
#include <iostream>
#include <algorithm>

WaveManager::WaveManager(const WaveDatabase& db, int level)
    : mDatabase(db), mCurrentWaveIndex(-1)
{
    mLevelConfig = mDatabase.getLevelConfig(level);
    if (!mLevelConfig) {
        std::cout << "No config found for level " << level << "\n";
        return;
    }

    const auto& allPaths = getPaths();
    if (level - 1 < (int)allPaths.size())
        mPaths = allPaths[level - 1];
    else
        std::cout << "No paths defined for level " << level << "\n";
}

void WaveManager::startNextWave() {
    if (!mLevelConfig) return;

    mCurrentWaveIndex++;
    if (mCurrentWaveIndex >= (int)mLevelConfig->waves.size()) return;

    const auto& config = mLevelConfig->waves[mCurrentWaveIndex];

    std::vector<Wave::EnemyTypeInfo> waveEnemyTypes;
    for (const auto& dbEnemy : config.enemyGroups) {
        Wave::EnemyTypeInfo e;
        e.type = dbEnemy.type;
        e.count = dbEnemy.count;
        e.spawnInterval = dbEnemy.interval;
        e.pathNumber = dbEnemy.pathNumber;
        waveEnemyTypes.push_back(e);
    }

    mCurrentWave = std::make_unique<Wave>(
        mLevelConfig->level,
        config.waveNumber,
        mPaths,
        waveEnemyTypes
    );
}

void WaveManager::update(float dt, std::vector<Enemy>& enemies) {
    if (mCurrentWave)
        mCurrentWave->update(dt, enemies);
}

bool WaveManager::isWaveComplete() const {
    return mCurrentWave && mCurrentWave->isComplete();
}

bool WaveManager::isLevelComplete() const {
    return mLevelConfig && mCurrentWaveIndex >= (int)mLevelConfig->waves.size() - 1 && isWaveComplete();
}

