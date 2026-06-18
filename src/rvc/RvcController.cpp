#include "rvc/RvcController.hpp"

#include "rvc/CleaningService.hpp"
#include "rvc/DustDetector.hpp"
#include "rvc/MapModel.hpp"
#include "rvc/MovementController.hpp"
#include "rvc/ObstacleDetector.hpp"

namespace rvc {

RvcController::RvcController(MapModel& map, ObstacleDetector& obstacleDetector,
                             DustDetector& dustDetector)
    : map_(map),
      obstacleDetector_(obstacleDetector),
      dustDetector_(dustDetector),
      movement_(map, cleaning_, obstacleDetector_) {}

void RvcController::startAutomaticCleaning() {
    cleaning_.activateCleaningAndMopping();
    movement_.moveForwardWithCleaning();
}

void RvcController::handleObstacleDetected() {
    cleaning_.stopCleaning();
    turnAsideAndResume();
}

void RvcController::handleSurroundedObstacle() {
    cleaning_.stopCleaning();
    movement_.moveBackward();
    turnAsideAndResume();
}

void RvcController::handleDustDetected() {
    if (!dustDetector_.isDustPresent()) {
        return;
    }
    cleaning_.boostCleaningPower(3);
    movement_.moveForwardWithCleaning();
}

void RvcController::onSimulationTick() {
    cleaning_.onSimulationTick();
}

void RvcController::turnAsideAndResume() {
    if (movement_.canTurnRight()) {
        movement_.turnRight();
    } else {
        movement_.turnLeft();
    }
    movement_.resumeForwardWithCleaning();
}

RvcSnapshot RvcController::snapshot() const {
    return map_.buildSnapshot(movement_.movementKind(), cleaning_.isCleaningActive(),
                              cleaning_.isMoppingActive(), cleaning_.level(),
                              cleaning_.boostRemainingSec());
}

}  // namespace rvc
