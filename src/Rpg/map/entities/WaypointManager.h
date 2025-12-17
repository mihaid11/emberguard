#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include <regex>
#include <iostream>
#include <nlohmann/json.hpp>

struct Waypoint {
    std::string mapPath;
    sf::Vector2f pos;
    bool isInterior;
};

class WaypointManager {
public:
    void loadWaypoints(const std::string& directory);
    const Waypoint* getWaypoint(const std::string& name) const;

private:
    std::map<std::string, Waypoint> mWaypoints;

    void parseMapFile(const std::string& path);
};

