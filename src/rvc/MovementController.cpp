#include "rvc/MovementController.hpp"

#include "rvc/CleaningService.hpp"
#include "rvc/MapModel.hpp"

namespace rvc {

MovementController::MovementController(MapModel& map, CleaningService& cleaning,
                                       IObstacleDetector& obstacleDetector)
    : map_(map),
      cleaning_(cleaning),
      obstacleDetector_(obstacleDetector),
      heading_(map.rvcHeading()),
      movementKind_(MovementKind::Idle) {}

void MovementController::moveForwardWithCleaning() {
    heading_ = map_.rvcHeading();
    movementKind_ = MovementKind::Forward;
    moveOneStep(heading_);
    cleaning_.syncForwardCleaningState();
}

void MovementController::moveBackward() {
    heading_ = map_.rvcHeading();
    movementKind_ = MovementKind::Backward;
    int back = (static_cast<int>(heading_) + 2) % 4;
    moveOneStep(static_cast<Direction>(back));
}

void MovementController::turnRight() {
    heading_ = map_.rvcHeading();
    movementKind_ = MovementKind::Turning;
    heading_ = static_cast<Direction>((static_cast<int>(heading_) + 1) % 4);
    map_.setRvcHeading(heading_);
}

void MovementController::turnLeft() {
    heading_ = map_.rvcHeading();
    movementKind_ = MovementKind::Turning;
    heading_ = static_cast<Direction>((static_cast<int>(heading_) + 3) % 4);
    map_.setRvcHeading(heading_);
}

bool MovementController::canTurnRight() {
    heading_ = map_.rvcHeading();
    return obstacleDetector_.isRightTurnFeasible();
}

void MovementController::resumeForwardWithCleaning() {
    heading_ = map_.rvcHeading();
    movementKind_ = MovementKind::Forward;
    moveOneStep(heading_);
    cleaning_.syncForwardCleaningState();
}

void MovementController::moveOneStep(Direction dir) {
    Position next = map_.rvcPosition();
    Position delta = offsetForDirection(dir);
    next.x += delta.x;
    next.y += delta.y;
    if (map_.isInBounds(next) && !map_.isObstacle(next)) {
        map_.setRvcPosition(next);
        map_.recordPathPoint(next);
    }
    heading_ = map_.rvcHeading();
}

}  // namespace rvc
