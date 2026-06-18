#include "rvc/ObstacleDetector.hpp"

#include "rvc/MapModel.hpp"

namespace rvc {

ObstacleDetector::ObstacleDetector(const MapModel& map) : map_(map) {}

bool ObstacleDetector::isRightTurnFeasible() const {
    Position right = map_.cellToRight();
    if (!map_.isInBounds(right)) {
        return false;
    }
    return !map_.isObstacle(right);
}

}  // namespace rvc
