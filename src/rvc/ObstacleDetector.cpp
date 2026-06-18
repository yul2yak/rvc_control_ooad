#include "rvc/ObstacleDetector.hpp"

#include "rvc/MapModel.hpp"

namespace rvc {

ObstacleDetector::ObstacleDetector(ISensorReader& sensor) : sensor_(sensor) {}

Direction ObstacleDetector::turnRight(Direction heading) {
    return static_cast<Direction>((static_cast<int>(heading) + 1) % 4);
}

bool ObstacleDetector::isRightTurnFeasible() const {
    const Direction rightHeading = turnRight(sensor_.currentHeading());
    return !sensor_.isFrontBlockedAtHeading(rightHeading);
}

bool ObstacleDetector::isSurroundedAt(const Position& pos, Direction heading) const {
    const Direction rightHeading = turnRight(heading);
    return sensor_.isFrontBlockedAt(pos, heading) && sensor_.isLeftBlockedAt(pos, heading) &&
           sensor_.isFrontBlockedAt(pos, rightHeading);
}

bool ObstacleDetector::isSurrounded() const {
    return isSurroundedAt(sensor_.currentPosition(), sensor_.currentHeading());
}

bool ObstacleDetector::wouldForwardLeadToSurroundedFrom(Direction heading) const {
    const Position delta = offsetForDirection(heading);
    Position front = sensor_.currentPosition();
    front.x += delta.x;
    front.y += delta.y;
    if (sensor_.isFrontBlockedAt(sensor_.currentPosition(), heading)) {
        return false;
    }
    return isSurroundedAt(front, heading);
}

bool ObstacleDetector::wouldForwardLeadToSurrounded() const {
    return wouldForwardLeadToSurroundedFrom(sensor_.currentHeading());
}

}  // namespace rvc
