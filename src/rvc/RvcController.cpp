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

void RvcController::resetManeuver() {
    maneuverPhase_ = ManeuverPhase::Idle;
    turnAttemptsFront_ = 0;
    turnAttemptsSurrounded_ = 0;
    firstTurnDone_ = false;
}

bool RvcController::maneuverInProgress() const {
    return maneuverPhase_ != ManeuverPhase::Idle;
}

void RvcController::activateCleaningSession() {
    cleaning_.activateCleaningAndMopping();
}

void RvcController::startAutomaticCleaning() {
    activateCleaningSession();
    movement_.moveForwardWithCleaning();
}

void RvcController::beginObstacleAvoidance() {
    cleaning_.stopCleaning();
    maneuverPhase_ = ManeuverPhase::TurnAside;
    turnAttemptsFront_ = 0;
    turnAttemptsSurrounded_ = 0;
    firstTurnDone_ = false;
}

void RvcController::beginSurroundedAvoidance() {
    cleaning_.stopCleaning();
    maneuverPhase_ = ManeuverPhase::Backward;
    turnAttemptsFront_ = 0;
    turnAttemptsSurrounded_ = 0;
    firstTurnDone_ = false;
}

void RvcController::doTurnOnce() {
    if (movement_.canTurnRight()) {
        movement_.turnRight();
    } else {
        movement_.turnLeft();
    }
}

bool RvcController::stepManeuver() {
    switch (maneuverPhase_) {
        case ManeuverPhase::Backward:
            movement_.moveBackward();
            maneuverPhase_ = ManeuverPhase::TurnAside;
            return true;
        case ManeuverPhase::TurnAside:
            if (!firstTurnDone_) {
                doTurnOnce();
                firstTurnDone_ = true;
                return true;
            }
            if (map_.isFrontBlocked() && turnAttemptsFront_ < 3) {
                doTurnOnce();
                ++turnAttemptsFront_;
                return true;
            }
            if (map_.isSurrounded() && turnAttemptsSurrounded_ < 4) {
                doTurnOnce();
                ++turnAttemptsSurrounded_;
                return true;
            }
            if (!map_.isFrontBlocked()) {
                maneuverPhase_ = ManeuverPhase::Forward;
                return true;
            }
            maneuverPhase_ = ManeuverPhase::Idle;
            return false;
        case ManeuverPhase::Forward:
            movement_.resumeForwardWithCleaning();
            maneuverPhase_ = ManeuverPhase::Idle;
            return false;
        case ManeuverPhase::Idle:
            return false;
    }
    return false;
}

void RvcController::handleObstacleDetected() {
    beginObstacleAvoidance();
    while (stepManeuver()) {
    }
}

void RvcController::handleSurroundedObstacle() {
    beginSurroundedAvoidance();
    while (stepManeuver()) {
    }
}

void RvcController::handleDustDetected() {
    if (!dustDetector_.isDustPresent()) {
        return;
    }
    cleaning_.boostCleaningPower(3);
    map_.clearDustAt(map_.rvcPosition());
    movement_.moveForwardWithCleaning();
}

void RvcController::onSimulationTick() {
    cleaning_.onSimulationTick();
}

void RvcController::turnAsideAndResume() {
    beginObstacleAvoidance();
    while (stepManeuver()) {
    }
}

RvcSnapshot RvcController::snapshot() const {
    return map_.buildSnapshot(movement_.movementKind(), cleaning_.isCleaningActive(),
                              cleaning_.isMoppingActive(), cleaning_.level(),
                              cleaning_.boostRemainingSec());
}

}  // namespace rvc
