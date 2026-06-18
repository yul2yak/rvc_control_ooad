#pragma once

#include "rvc/CleaningService.hpp"
#include "rvc/DustDetector.hpp"
#include "rvc/MapModel.hpp"
#include "rvc/MovementController.hpp"
#include "rvc/ObstacleDetector.hpp"

namespace rvc {

class SimulationEngine;

class RvcController {
public:
    RvcController(MapModel& map, ObstacleDetector& obstacleDetector, DustDetector& dustDetector);

    void startAutomaticCleaning();
    void handleObstacleDetected();
    void handleSurroundedObstacle();
    void handleDustDetected();

    void onSimulationTick();

    RvcSnapshot snapshot() const;
    MapModel& map() { return map_; }
    const MapModel& map() const { return map_; }

    friend class SimulationEngine;

private:
    MapModel& map_;
    CleaningService cleaning_;
    ObstacleDetector& obstacleDetector_;
    DustDetector& dustDetector_;
    MovementController movement_;

    void turnAsideAndResume();
};

}  // namespace rvc
