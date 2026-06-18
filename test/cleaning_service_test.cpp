#include "rvc/CleaningService.hpp"

#include <gtest/gtest.h>

// FR-001, FR-002
TEST(CleaningServiceTest, ActivateCleaningAndMopping_FR001) {
    rvc::CleaningService clean;
    clean.activateCleaningAndMopping();
    EXPECT_TRUE(clean.isCleaningActive());
    EXPECT_TRUE(clean.isMoppingActive());
}

// FR-002, section 0.4
TEST(CleaningServiceTest, StopCleaning_FR002) {
    rvc::CleaningService clean;
    clean.activateCleaningAndMopping();
    clean.stopCleaning();
    EXPECT_FALSE(clean.isCleaningActive());
    EXPECT_FALSE(clean.isMoppingActive());
}

// FR-002, section 0.4
TEST(CleaningServiceTest, SyncForwardCleaningState_FR002) {
    rvc::CleaningService clean;
    clean.stopCleaning();
    clean.syncForwardCleaningState();
    EXPECT_TRUE(clean.isCleaningActive());
    EXPECT_TRUE(clean.isMoppingActive());
}

// FR-005, UR-003
TEST(CleaningServiceTest, BoostCleaningPower_FR005_UR003) {
    rvc::CleaningService clean;
    clean.boostCleaningPower(3);
    EXPECT_EQ(clean.level(), rvc::OutputLevel::Boosted);
    EXPECT_EQ(clean.boostRemainingSec(), 3);
}

// NFR-004
TEST(CleaningServiceTest, BoostDurationConfigurable_NFR004) {
    rvc::CleaningService clean;
    clean.boostCleaningPower(5);
    EXPECT_EQ(clean.boostRemainingSec(), 5);
}

// FR-005, NFR-004
TEST(CleaningServiceTest, RestoreDefaultCleaningPower_FR005) {
    rvc::CleaningService clean;
    clean.boostCleaningPower(3);
    clean.restoreDefaultCleaningPower();
    EXPECT_EQ(clean.level(), rvc::OutputLevel::Default);
    EXPECT_EQ(clean.boostRemainingSec(), 0);
}

// FR-005
TEST(CleaningServiceTest, BoostExpiresOnTick_FR005) {
    rvc::CleaningService clean;
    clean.boostCleaningPower(2);
    clean.onSimulationTick();
    EXPECT_EQ(clean.boostRemainingSec(), 1);
    clean.onSimulationTick();
    EXPECT_EQ(clean.level(), rvc::OutputLevel::Default);
}
