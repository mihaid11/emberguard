#include "Wave.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "PathsConfig.h"

Wave::Wave(int level, int waveNumber, const std::vector<Path>& paths,
           const std::vector<EnemyTypeInfo>& enemyTypes)
    : mTotalEnemies(0), mEnemiesSpawned(0), mPaths(paths), mGlobalSpawnTimer(0.0f) {
    mEnemyTypes = enemyTypes;
    for (const auto& info : mEnemyTypes) {
        mEnemiesSpawnedPerType.push_back(0);
        mTotalEnemies += info.count;
    }
}

void Wave::update(float dt, std::vector<Enemy>& enemies) {
    mGlobalSpawnTimer += dt;
    for (size_t i = 0; i < mEnemyTypes.size(); ++i) {
        const auto& info = mEnemyTypes[i];

        if (mEnemiesSpawnedPerType[i] < info.count &&
            mGlobalSpawnTimer >= info.spawnInterval * (mEnemiesSpawnedPerType[i] + 1)) {

            const Path& path = mPaths[info.pathNumber % mPaths.size()];
            enemies.push_back(createEnemy(info.type, path));

            mEnemiesSpawnedPerType[i]++;
            mEnemiesSpawned++;
        }
    }
}

bool Wave::isComplete() const {
    return mEnemiesSpawned >= mTotalEnemies;
}

int Wave::getEnemyCount() const {
    return mTotalEnemies;
}

Enemy Wave::createEnemy(const std::string& type, const Path& path) const {
    if (type == "WastelandMarauder")
        return WastelandMarauder(path);
    else if (type == "RadiationBerserker")
        return RadiationBerserker(path);
    else if (type == "EmpireScout")
        return EmpireScout(path);

    // Default fallback
    return WastelandMarauder(path);
}

