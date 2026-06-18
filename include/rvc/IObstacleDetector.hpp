#pragma once

namespace rvc {

class IObstacleDetector {
public:
    virtual ~IObstacleDetector() = default;
    virtual bool isRightTurnFeasible() const = 0;
};

}  // namespace rvc
