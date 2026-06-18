#pragma once

#include "rvc/IObstacleDetector.hpp"
#include "rvc/Types.hpp"

namespace rvc {

class CleaningService;
class MapModel;

class MovementController {
public:
    MovementController(MapModel& map, CleaningService& cleaning, IObstacleDetector& obstacleDetector);

    void moveForwardWithCleaning();
    void moveBackward();
    void turnRight();
    void turnLeft();
    bool canTurnRight();
    void resumeForwardWithCleaning();

    Direction heading() const { return heading_; }
    MovementKind movementKind() const { return movementKind_; }

private:
    MapModel& map_;
    CleaningService& cleaning_;
    IObstacleDetector& obstacleDetector_;
    Direction heading_;
    MovementKind movementKind_;

    void moveOneStep(Direction dir);
};

}  // namespace rvc
