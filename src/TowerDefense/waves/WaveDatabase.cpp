#include "WaveDatabase.h"
#include <fstream>
#include <iostream>

bool WaveDatabase::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open waves file: " << filepath << "\n";
        return false;
    }

    json j;
    file >> j;

    if (!j.contains("levels") || !j["levels"].is_array()) {
        std::cerr << "Invalid waves.json format\n";
        return false;
    }

    for (const auto& levelJson : j["levels"]) {
        LevelConfig level;
        level.level = levelJson.value("level", 0);

        for (const auto& waveJson : levelJson["waves"]) {
            WaveConfig wave;
            wave.waveNumber = waveJson.value("waveNumber", 0);

            for (const auto& enemyJson : waveJson["enemyGroups"]) {
                EnemyTypeInfo info;
                info.type = enemyJson.value("type", "");
                info.count = enemyJson.value("count", 0);
                info.interval = enemyJson.value("interval", 1.0f);
                info.pathNumber = enemyJson.value("path", 0);
                wave.enemyGroups.push_back(info);
            }

            level.waves.push_back(wave);
        }

        mLevels.push_back(level);
    }

    return true;
}

const LevelConfig* WaveDatabase::getLevelConfig(int level) const {
    for (const auto& lvl : mLevels)
        if (lvl.level == level)
            return &lvl;
    return nullptr;
}

