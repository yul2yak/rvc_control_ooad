#pragma once

#include "rvc/Types.hpp"

#include <string>
#include <vector>

namespace rvc {

struct RvcSnapshot {
    Position position;
    Direction heading = Direction::North;
    MovementKind movementKind = MovementKind::Idle;
    bool cleaningActive = false;
    bool moppingActive = false;
    OutputLevel level = OutputLevel::Default;
    int boostRemainingSec = 0;
};

class MapModel {
public:
    void setSize(int width, int height);
    int width() const { return width_; }
    int height() const { return height_; }

    void setObstacles(const std::vector<Position>& obstacles);
    void setDust(const std::vector<Position>& dust);
    void setRvcStart(const Position& pos, Direction heading);

    bool isObstacle(const Position& pos) const;
    bool hasDust(const Position& pos) const;
    void clearDustAt(const Position& pos);
    const std::vector<Position>& dustCells() const { return dust_; }

    bool isFrontBlocked() const;
    bool isSurrounded() const;

    Position rvcPosition() const { return rvcPos_; }
    Direction rvcHeading() const { return heading_; }
    void setRvcPosition(const Position& pos) { rvcPos_ = pos; }
    void setRvcHeading(Direction heading) { heading_ = heading; }

    Position cellInFront() const;
    Position cellToLeft() const;
    Position cellToRight() const;
    Position cellBehind() const;

    bool isInBounds(const Position& pos) const;
    void recordPathPoint(const Position& pos);
    const std::vector<Position>& path() const { return path_; }
    void clearPath();

    void applySnapshotFields(const RvcSnapshot& snapshot);
    RvcSnapshot buildSnapshot(MovementKind kind, bool cleaningActive, bool moppingActive,
                              OutputLevel level, int boostRemainingSec) const;

private:
    int width_ = 0;
    int height_ = 0;
    Position rvcPos_;
    Direction heading_ = Direction::North;
    std::vector<Position> obstacles_;
    std::vector<Position> dust_;
    std::vector<Position> path_;
};

std::string directionToString(Direction d);
Direction directionFromString(const std::string& s);
Position offsetForDirection(Direction d);

}  // namespace rvc
