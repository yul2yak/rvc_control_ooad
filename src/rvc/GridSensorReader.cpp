#include "rvc/GridSensorReader.hpp"

#include "rvc/MapModel.hpp"

namespace rvc {

GridSensorReader::GridSensorReader(const MapModel& map) : map_(map) {}

Direction GridSensorReader::currentHeading() const {
    return map_.rvcHeading();
}

Position GridSensorReader::currentPosition() const {
    return map_.rvcPosition();
}

bool GridSensorReader::isFrontBlockedAt(const Position& pos, Direction heading) const {
    Position front = pos;
    const Position d = offsetForDirection(heading);
    front.x += d.x;
    front.y += d.y;
    return !map_.isInBounds(front) || map_.isObstacle(front);
}

bool GridSensorReader::isLeftBlockedAt(const Position& pos, Direction heading) const {
    const int left = (static_cast<int>(heading) + 3) % 4;
    Position cell = pos;
    const Position d = offsetForDirection(static_cast<Direction>(left));
    cell.x += d.x;
    cell.y += d.y;
    return !map_.isInBounds(cell) || map_.isObstacle(cell);
}

bool GridSensorReader::isFrontBlocked() const {
    return isFrontBlockedAt(map_.rvcPosition(), map_.rvcHeading());
}

bool GridSensorReader::isLeftBlocked() const {
    return isLeftBlockedAt(map_.rvcPosition(), map_.rvcHeading());
}

bool GridSensorReader::isFrontBlockedAtHeading(Direction heading) const {
    return isFrontBlockedAt(map_.rvcPosition(), heading);
}

}  // namespace rvc
