#pragma once

namespace rvc {

enum class Direction { North = 0, East = 1, South = 2, West = 3 };

enum class MovementKind { Idle, Forward, Backward, Turning };

enum class OutputLevel { Default, Boosted };

struct Position {
    int x = 0;
    int y = 0;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

}  // namespace rvc
