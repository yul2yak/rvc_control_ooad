#pragma once

#include "rvc/IObstacleDetector.hpp"
#include "rvc/ISensorReader.hpp"

namespace rvc {

class ObstacleDetector : public IObstacleDetector {
public:
    explicit ObstacleDetector(ISensorReader& sensor);

    bool isRightTurnFeasible() const override;

    bool isSurrounded() const;
    bool isSurroundedAt(const Position& pos, Direction heading) const;
    bool wouldForwardLeadToSurroundedFrom(Direction heading) const;
    bool wouldForwardLeadToSurrounded() const;

private:
    ISensorReader& sensor_;

    static Direction turnRight(Direction heading);
};

}  // namespace rvc
