#pragma once

#include "rvc/IObstacleDetector.hpp"

namespace rvc {

class MapModel;

class ObstacleDetector : public IObstacleDetector {
public:
    explicit ObstacleDetector(const MapModel& map);

    bool isRightTurnFeasible() const override;

private:
    const MapModel& map_;
};

}  // namespace rvc
