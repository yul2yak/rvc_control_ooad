#include "rvc/CleaningService.hpp"

namespace rvc {

void CleaningService::activateCleaningAndMopping() {
    cleaningActive_ = true;
    moppingActive_ = true;
}

void CleaningService::stopCleaning() {
    cleaningActive_ = false;
    moppingActive_ = false;
}

void CleaningService::syncForwardCleaningState() {
    cleaningActive_ = true;
    moppingActive_ = true;
}

void CleaningService::boostCleaningPower(int durationSec) {
    level_ = OutputLevel::Boosted;
    boostRemainingSec_ = durationSec;
}

void CleaningService::restoreDefaultCleaningPower() {
    level_ = OutputLevel::Default;
    boostRemainingSec_ = 0;
}

void CleaningService::onSimulationTick() {
    if (boostRemainingSec_ <= 0) {
        return;
    }
    --boostRemainingSec_;
    if (boostRemainingSec_ <= 0) {
        restoreDefaultCleaningPower();
    }
}

}  // namespace rvc
