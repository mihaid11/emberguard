#include "WaypointManager.h"

void WaypointManager::loadWaypoints(const std::string& directory) {
    for (const auto& entry: std::filesystem::recursive_directory_iterator(directory))
        if (entry.path().extension() == ".json")
            parseMapFile(entry.path().string());
}

const Waypoint* WaypointManager::getWaypoint(const std::string& name) const {
    if (mWaypoints.count(name))
        return &mWaypoints.at(name);
    return nullptr;
}

void WaypointManager::parseMapFile(const std::string& path) {
    std::ifstream file(path);

    try {
        nlohmann::json j;
        file >> j;

        bool isInterior = (path.find("interiors") != std::string::npos);

        float offsetX = 0.f;
        float offsetY = 0.f;

        if (!isInterior) {
            std::regex re("map_(-?\\d+)_(-?\\d+)\\.json");
            std::smatch match;

            std::string filename = std::filesystem::path(path).filename().string();
            if (std::regex_search(filename, match, re)) {
                int chunkX = std::stoi(match[1]);
                int chunkY = std::stoi(match[2]);

                offsetX = chunkX * 1024.f;
                offsetY = chunkY * 1024.f;
            }
        } else {
            offsetX -= 1024.f;
            offsetY -= 1024.f;
        }

        if (j.contains("layers")) {
            for (const auto& layer: j["layers"]) {
                if (layer["type"] == "objectgroup" && layer.contains("objects")) {
                    for (const auto& object: layer["objects"]) {
                        std::string type = object.value("type", "");

                        if (type == "Waypoint") {
                            std::string name = object.value("name", "");
                            float x = object.value("x", 0.f);
                            float y = object.value("y", 0.f);

                            if (!name.empty()) {
                                Waypoint waypoint;

                                waypoint.pos = sf::Vector2f(x + offsetX, y + offsetY);
                                waypoint.mapPath = isInterior ? path: "open_world";
                                waypoint.isInterior = isInterior;

                                mWaypoints[name] = waypoint;
                            }
                        } else if (type == "NPCWaypoint") {
                            std::string npcId = object.value("name", "");
                            float x = object.value("x", 0.f) + offsetX;
                            float y = object.value("y", 0.f) + offsetY;

                            int index = 0;
                            if (object.contains("properties")) {
                                for (const auto& prop: object["properties"]) {
                                    if (prop["name"] == "index") {
                                        index = prop["value"];
                                        break;
                                    }
                                }
                            }

                            if (!npcId.empty())
                                mNPCPaths[npcId][index] = sf::Vector2f(x, y);
                        }
                    }
                }
            }
        }
    } catch (...) {
        std::cerr << "Failed to parse waypoints: " << path << std::endl;
    }
}

std::vector<sf::Vector2f> WaypointManager::getNPCPath(const std::string& npcId) const {
    std::vector<sf::Vector2f> path;
    auto it = mNPCPaths.find(npcId);

    if (it != mNPCPaths.end())
        for (const auto& [index, position] : it->second)
            path.push_back(position);
    return path;
}
