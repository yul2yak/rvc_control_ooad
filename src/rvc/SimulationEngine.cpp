#include "rvc/SimulationEngine.hpp"

#include "rvc/MovementController.hpp"

namespace rvc {

EnvironmentEvent detectEnvironmentEvent(const MapModel& map) {
    if (map.isSurrounded()) {
        return EnvironmentEvent::Surrounded;
    }
    if (map.isFrontBlocked()) {
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

void SimulationEngine::reloadLayout(const MapScenario& scenario) {
    scenario_ = scenario;
    map_.setSize(scenario.width, scenario.height);
    map_.setObstacles(scenario.obstacles);
    map_.setDust(scenario.dust);
}

void SimulationEngine::loadScenario(const MapScenario& scenario) {
    scenario_ = scenario;
    applyScenarioToMap(scenario_, map_);
    sessionActive_ = false;
    controller_.resetManeuver();
}

MapScenario SimulationEngine::currentScenario() const {
    MapScenario s = scenario_;
    s.rvcStart = map_.rvcPosition();
    s.rvcHeading = map_.rvcHeading();
    return s;
}

bool SimulationEngine::isSessionActive() const {
    return sessionActive_;
}

void SimulationEngine::triggerStart() {
    sessionActive_ = true;
    controller_.activateCleaningSession();
}

void SimulationEngine::triggerObstacle() {
    sessionActive_ = true;
    controller_.beginObstacleAvoidance();
    controller_.stepManeuver();
}

void SimulationEngine::triggerSurrounded() {
    sessionActive_ = true;
    controller_.beginSurroundedAvoidance();
    controller_.stepManeuver();
}

void SimulationEngine::triggerDust() {
    sessionActive_ = true;
    controller_.handleDustDetected();
}

void SimulationEngine::processEnvironmentEvent() {
    switch (detectEnvironmentEvent(map_)) {
        case EnvironmentEvent::Surrounded:
            controller_.beginSurroundedAvoidance();
            controller_.stepManeuver();
            break;
        case EnvironmentEvent::ObstacleFront:
            controller_.beginObstacleAvoidance();
            controller_.stepManeuver();
            break;
        case EnvironmentEvent::Dust:
            controller_.handleDustDetected();
            break;
        case EnvironmentEvent::None:
            controller_.movement_.moveForwardWithCleaning();
            break;
    }
}

void SimulationEngine::autoTickStep() {
    if (!sessionActive_) {
        return;
    }
    controller_.onSimulationTick();
    if (controller_.maneuverInProgress()) {
        controller_.stepManeuver();
        return;
    }
    processEnvironmentEvent();
}

void SimulationEngine::tickOnce() {
    if (!sessionActive_) {
        if (scenario_.trigger == "obstacle") {
            triggerObstacle();
        } else if (scenario_.trigger == "surrounded") {
            triggerSurrounded();
        } else if (scenario_.trigger == "dust") {
            triggerDust();
        } else {
            triggerStart();
            autoTickStep();
        }
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
