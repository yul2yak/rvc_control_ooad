#include "rvc/MapModel.hpp"

#include <algorithm>

namespace rvc {

namespace {

bool contains(const std::vector<Position>& list, const Position& pos) {
    return std::find(list.begin(), list.end(), pos) != list.end();
}

}  // namespace

void MapModel::setSize(int width, int height) {
    width_ = width;
    height_ = height;
}

void MapModel::setObstacles(const std::vector<Position>& obstacles) {
    obstacles_ = obstacles;
}

void MapModel::setDust(const std::vector<Position>& dust) {
    dust_ = dust;
}

void MapModel::setRvcStart(const Position& pos, Direction heading) {
    rvcPos_ = pos;
    heading_ = heading;
    path_.clear();
    path_.push_back(pos);
}

bool MapModel::isObstacle(const Position& pos) const {
    return contains(obstacles_, pos);
}

bool MapModel::hasDust(const Position& pos) const {
    return contains(dust_, pos);
}

void MapModel::clearDustAt(const Position& pos) {
    dust_.erase(std::remove(dust_.begin(), dust_.end(), pos), dust_.end());
}

Position MapModel::cellInFront() const {
    Position p = rvcPos_;
    Position d = offsetForDirection(heading_);
    p.x += d.x;
    p.y += d.y;
    return p;
}

Position MapModel::cellToLeft() const {
    int left = (static_cast<int>(heading_) + 3) % 4;
    Position p = rvcPos_;
    Position d = offsetForDirection(static_cast<Direction>(left));
    p.x += d.x;
    p.y += d.y;
    return p;
}

Position MapModel::cellToRight() const {
    int right = (static_cast<int>(heading_) + 1) % 4;
    Position p = rvcPos_;
    Position d = offsetForDirection(static_cast<Direction>(right));
    p.x += d.x;
    p.y += d.y;
    return p;
}

Position MapModel::cellBehind() const {
    int back = (static_cast<int>(heading_) + 2) % 4;
    Position p = rvcPos_;
    Position d = offsetForDirection(static_cast<Direction>(back));
    p.x += d.x;
    p.y += d.y;
    return p;
}

bool MapModel::isInBounds(const Position& pos) const {
    return pos.x >= 0 && pos.y >= 0 && pos.x < width_ && pos.y < height_;
}

void MapModel::recordPathPoint(const Position& pos) {
    if (path_.empty() || path_.back() != pos) {
        path_.push_back(pos);
    }
}

void MapModel::clearPath() {
    path_.clear();
    path_.push_back(rvcPos_);
}

void MapModel::applySnapshotFields(const RvcSnapshot& snapshot) {
    rvcPos_ = snapshot.position;
    heading_ = snapshot.heading;
}

RvcSnapshot MapModel::buildSnapshot(MovementKind kind, bool cleaningActive, bool moppingActive,
                                    OutputLevel level, int boostRemainingSec) const {
    RvcSnapshot s;
    s.position = rvcPos_;
    s.heading = heading_;
    s.movementKind = kind;
    s.cleaningActive = cleaningActive;
    s.moppingActive = moppingActive;
    s.level = level;
    s.boostRemainingSec = boostRemainingSec;
    return s;
}

std::string directionToString(Direction d) {
    switch (d) {
        case Direction::North:
            return "N";
        case Direction::East:
            return "E";
        case Direction::South:
            return "S";
        case Direction::West:
            return "W";
    }
    return "N";
}

Direction directionFromString(const std::string& s) {
    if (s == "E") return Direction::East;
    if (s == "S") return Direction::South;
    if (s == "W") return Direction::West;
    return Direction::North;
}

Position offsetForDirection(Direction d) {
    switch (d) {
        case Direction::North:
            return {0, -1};
        case Direction::East:
            return {1, 0};
        case Direction::South:
            return {0, 1};
        case Direction::West:
            return {-1, 0};
    }
    return {0, -1};
}

}  // namespace rvc
