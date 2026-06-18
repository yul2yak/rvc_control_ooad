#pragma once

#include "rvc/MapModel.hpp"
#include "rvc/RvcController.hpp"

#include <string>
#include <vector>

namespace rvc {

struct MapScenario {
    std::string id;
    std::string name;
    int width = 0;
    int height = 0;
    Position rvcStart;
    Direction rvcHeading = Direction::North;
    std::vector<Position> obstacles;
    std::vector<Position> dust;
    int ticks = 1;
    std::string trigger = "start";
    std::vector<Position> expectedPath;
    std::vector<std::string> expectedSnapshots;
};

bool loadScenarioFromFile(const std::string& path, MapScenario& out);
bool saveScenarioToFile(const std::string& path, const MapScenario& scenario);
std::string scenarioToJson(const MapScenario& scenario);

void applyScenarioToMap(const MapScenario& scenario, MapModel& map);

}  // namespace rvc
