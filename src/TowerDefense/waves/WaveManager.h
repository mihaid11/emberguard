#pragma once
#include "WaveDatabase.h"
#include "Wave.h"
#include "PathsConfig.h"
#include <memory>

class WaveManager {
public:
    WaveManager(const WaveDatabase& db, int level);

    void startNextWave();
    void update(float dt, std::vector<Enemy>& enemies);

    bool isWaveComplete() const;
    bool isLevelComplete() const;

private:
    const WaveDatabase& mDatabase;
    const LevelConfig* mLevelConfig;
    int mCurrentWaveIndex;
    std::unique_ptr<Wave> mCurrentWave;
    std::vector<Path> mPaths;
};

