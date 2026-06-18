#pragma once

#include "rvc/Types.hpp"

namespace rvc {

class CleaningService {
public:
    void activateCleaningAndMopping();
    void stopCleaning();
    void syncForwardCleaningState();
    void boostCleaningPower(int durationSec);
    void restoreDefaultCleaningPower();

    void onSimulationTick();

    bool isCleaningActive() const { return cleaningActive_; }
    bool isMoppingActive() const { return moppingActive_; }
    OutputLevel level() const { return level_; }
    int boostRemainingSec() const { return boostRemainingSec_; }

private:
    bool cleaningActive_ = false;
    bool moppingActive_ = false;
    OutputLevel level_ = OutputLevel::Default;
    int boostRemainingSec_ = 0;
};

}  // namespace rvc
