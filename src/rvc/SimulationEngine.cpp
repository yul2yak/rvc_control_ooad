#include "rvc/SimulationEngine.hpp"

#include "rvc/MovementController.hpp"

namespace rvc {

EnvironmentEvent detectEnvironmentEvent(const MapModel& map) {
    const Position front = map.cellInFront();
    const Position left = map.cellToLeft();
    const Position right = map.cellToRight();

    const bool frontBlocked = !map.isInBounds(front) || map.isObstacle(front);
    const bool leftBlocked = !map.isInBounds(left) || map.isObstacle(left);
    const bool rightBlocked = !map.isInBounds(right) || map.isObstacle(right);

    if (frontBlocked && leftBlocked && rightBlocked) {
        return EnvironmentEvent::Surrounded;
    }
    if (frontBlocked) {
        return EnvironmentEvent::ObstacleFront;
    }
    if (map.hasDust(map.rvcPosition())) {
        return EnvironmentEvent::Dust;
    }
    return EnvironmentEvent::None;
}

SimulationEngine::SimulationEngine()
    : obstacleDetector_(map_),
      dustDetector_(map_),
      controller_(map_, obstacleDetector_, dustDetector_) {}

void SimulationEngine::loadScenario(const MapScenario& scenario) {
    scenario_ = scenario;
    applyScenarioToMap(scenario_, map_);
    sessionActive_ = false;
}

MapScenario SimulationEngine::currentScenario() const {
    MapScenario s = scenario_;
    s.rvcStart = map_.rvcPosition();
    s.rvcHeading = map_.rvcHeading();
    return s;
}

void SimulationEngine::triggerStart() {
    sessionActive_ = true;
    controller_.startAutomaticCleaning();
}

void SimulationEngine::triggerObstacle() {
    sessionActive_ = true;
    controller_.handleObstacleDetected();
}

void SimulationEngine::triggerSurrounded() {
    sessionActive_ = true;
    controller_.handleSurroundedObstacle();
}

void SimulationEngine::triggerDust() {
    sessionActive_ = true;
    controller_.handleDustDetected();
}

void SimulationEngine::autoTickStep() {
    if (!sessionActive_) {
        return;
    }
    controller_.onSimulationTick();
    const EnvironmentEvent event = detectEnvironmentEvent(map_);
    switch (event) {
        case EnvironmentEvent::Surrounded:
            controller_.handleSurroundedObstacle();
            break;
        case EnvironmentEvent::ObstacleFront:
            controller_.handleObstacleDetected();
            break;
        case EnvironmentEvent::Dust:
            controller_.handleDustDetected();
            map_.clearDustAt(map_.rvcPosition());
            break;
        case EnvironmentEvent::None:
            controller_.movement_.moveForwardWithCleaning();
            break;
    }
}

void SimulationEngine::tickOnce() {
    if (scenario_.trigger == "obstacle") {
        triggerObstacle();
    } else if (scenario_.trigger == "surrounded") {
        triggerSurrounded();
    } else if (scenario_.trigger == "dust") {
        triggerDust();
    } else if (!sessionActive_) {
        triggerStart();
    } else {
        autoTickStep();
    }
}

void SimulationEngine::runTicks(int count) {
    for (int i = 0; i < count; ++i) {
        tickOnce();
    }
}

bool SimulationEngine::pathMatchesExpected() const {
    if (scenario_.expectedPath.empty()) {
        return true;
    }
    const auto& actual = map_.path();
    if (actual.size() != scenario_.expectedPath.size()) {
        return false;
    }
    for (std::size_t i = 0; i < actual.size(); ++i) {
        if (actual[i] != scenario_.expectedPath[i]) {
            return false;
        }
    }
    return true;
}

}  // namespace rvc
