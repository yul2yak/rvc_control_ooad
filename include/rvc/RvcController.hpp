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
    void activateCleaningSession();
    void handleObstacleDetected();
    void handleSurroundedObstacle();
    void handleDustDetected();

    void onSimulationTick();

    RvcSnapshot snapshot() const;
    MapModel& map() { return map_; }
    const MapModel& map() const { return map_; }

    void resetManeuver();
    bool maneuverInProgress() const;

    friend class SimulationEngine;

private:
    enum class ManeuverPhase { Idle, Backward, TurnAside, Forward };

    MapModel& map_;
    CleaningService cleaning_;
    ObstacleDetector& obstacleDetector_;
    DustDetector& dustDetector_;
    MovementController movement_;

    ManeuverPhase maneuverPhase_ = ManeuverPhase::Idle;
    int turnAttemptsFront_ = 0;
    int turnAttemptsSurrounded_ = 0;
    bool firstTurnDone_ = false;

    void beginObstacleAvoidance();
    void beginSurroundedAvoidance();
    bool stepManeuver();
    void doTurnOnce();
    void turnAsideAndResume();
};

}  // namespace rvc
