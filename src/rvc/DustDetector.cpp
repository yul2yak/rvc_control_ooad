#include "rvc/DustDetector.hpp"

#include "rvc/MapModel.hpp"

namespace rvc {

DustDetector::DustDetector(const MapModel& map) : map_(map) {}

bool DustDetector::isDustPresent() const {
    return map_.hasDust(map_.rvcPosition());
}

}  // namespace rvc
