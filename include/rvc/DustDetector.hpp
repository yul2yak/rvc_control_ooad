#pragma once

#include "rvc/IDustDetector.hpp"

namespace rvc {

class MapModel;

class DustDetector : public IDustDetector {
public:
    explicit DustDetector(const MapModel& map);

    bool isDustPresent() const override;

private:
    const MapModel& map_;
};

}  // namespace rvc
