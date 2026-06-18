#pragma once

#include "rvc/ISensorReader.hpp"

namespace rvc {

class MapModel;

class GridSensorReader : public ISensorReader {
public:
    explicit GridSensorReader(const MapModel& map);

    Direction currentHeading() const override;
    Position currentPosition() const override;

    bool isFrontBlocked() const override;
    bool isLeftBlocked() const override;
    bool isFrontBlockedAtHeading(Direction heading) const override;
    bool isFrontBlockedAt(const Position& pos, Direction heading) const override;
    bool isLeftBlockedAt(const Position& pos, Direction heading) const override;

private:
    const MapModel& map_;
};

}  // namespace rvc
