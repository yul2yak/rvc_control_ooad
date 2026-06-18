#pragma once

#include "rvc/Types.hpp"

namespace rvc {

class ISensorReader {
public:
    virtual ~ISensorReader() = default;

    virtual Direction currentHeading() const = 0;
    virtual Position currentPosition() const = 0;

    virtual bool isFrontBlocked() const = 0;
    virtual bool isLeftBlocked() const = 0;

    /** Front sensor reading at current position with the given heading (virtual-pose). */
    virtual bool isFrontBlockedAtHeading(Direction heading) const = 0;

    virtual bool isFrontBlockedAt(const Position& pos, Direction heading) const = 0;
    virtual bool isLeftBlockedAt(const Position& pos, Direction heading) const = 0;
};

}  // namespace rvc
