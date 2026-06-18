#pragma once

#include "rvc/DustDetector.hpp"
#include "rvc/MapJson.hpp"
#include "rvc/ObstacleDetector.hpp"
#include "rvc/RvcController.hpp"

namespace rvc {

enum class EnvironmentEvent { None, ObstacleFront, Surrounded, Dust };

EnvironmentEvent detectEnvironmentEvent(const MapModel& map);

class SimulationEngine {
public:
    SimulationEngine();

    void loadScenario(const MapScenario& scenario);
    MapScenario currentScenario() const;
    RvcController& controller() { return controller_; }
    const RvcController& controller() const { return controller_; }
    MapModel& map() { return map_; }

    void triggerStart();
    void triggerObstacle();
    void triggerSurrounded();
    void triggerDust();
    void tickOnce();
    void runTicks(int count);

    bool pathMatchesExpected() const;

private:
    MapModel map_;
    ObstacleDetector obstacleDetector_;
    DustDetector dustDetector_;
    RvcController controller_;
    MapScenario scenario_;
    bool sessionActive_ = false;

    void autoTickStep();
};

}  // namespace rvc
