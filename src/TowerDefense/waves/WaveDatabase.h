#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../enemy/Enemy.h"

using json = nlohmann::json;

struct EnemyTypeInfo {
    std::string type;
    int count;
    float interval;
    int pathNumber;
};

struct WaveConfig {
    int waveNumber;
    std::vector<EnemyTypeInfo> enemyGroups;
};

struct LevelConfig {
    int level;
    std::vector<WaveConfig> waves;
};

class WaveDatabase {
public:
    bool loadFromFile(const std::string& filepath);
    const LevelConfig* getLevelConfig(int level) const;

private:
    std::vector<LevelConfig> mLevels;
};

