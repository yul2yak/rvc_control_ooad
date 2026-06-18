#include "rvc/DustDetector.hpp"
#include "rvc/ObstacleDetector.hpp"
#include "rvc/RvcController.hpp"

#include <gtest/gtest.h>

namespace {

rvc::MapModel makeMap(int w, int h, rvc::Position start, rvc::Direction heading) {
    rvc::MapModel map;
    map.setSize(w, h);
    map.setRvcStart(start, heading);
    return map;
}

}  // namespace

// FR-001, FR-002, NFR-002 — startAutomaticCleaning (SD-UC-001-S01)
TEST(RvcControllerTest, StartAutomaticCleaning_FR001_FR002) {
    auto map = makeMap(6, 6, {1, 3}, rvc::Direction::North);
    rvc::ObstacleDetector obs(map);
    rvc::DustDetector dust(map);
    rvc::RvcController ctrl(map, obs, dust);

    ctrl.startAutomaticCleaning();
    const rvc::RvcSnapshot snap = ctrl.snapshot();
    EXPECT_TRUE(snap.cleaningActive);
    EXPECT_TRUE(snap.moppingActive);
    EXPECT_EQ(snap.position, (rvc::Position{1, 2}));
}

// FR-003, UR-001 — handleObstacleDetected turn right (SD-UC-003-S01)
TEST(RvcControllerTest, HandleObstacleTurnRight_FR003) {
    auto map = makeMap(6, 6, {2, 3}, rvc::Direction::North);
    map.setObstacles({{2, 2}});
    rvc::ObstacleDetector obs(map);
    rvc::DustDetector dust(map);
    rvc::RvcController ctrl(map, obs, dust);

    ctrl.handleObstacleDetected();
    const rvc::RvcSnapshot snap = ctrl.snapshot();
    EXPECT_TRUE(snap.cleaningActive);
    EXPECT_EQ(snap.heading, rvc::Direction::East);
}

// FR-003, UR-001 — handleObstacleDetected turn left (SD-UC-003-S02)
TEST(RvcControllerTest, HandleObstacleTurnLeft_FR003) {
    auto map = makeMap(6, 6, {2, 3}, rvc::Direction::North);
    map.setObstacles({{2, 2}, {3, 3}});
    rvc::ObstacleDetector obs(map);
    rvc::DustDetector dust(map);
    rvc::RvcController ctrl(map, obs, dust);

    ctrl.handleObstacleDetected();
    EXPECT_EQ(ctrl.snapshot().heading, rvc::Direction::West);
}

// FR-004, UR-002 — handleSurroundedObstacle backward then turn (SD-UC-004-S01)
TEST(RvcControllerTest, HandleSurroundedBackward_FR004) {
    auto map = makeMap(6, 6, {2, 2}, rvc::Direction::North);
    map.setObstacles({{2, 1}, {1, 2}, {3, 2}});
    rvc::ObstacleDetector obs(map);
    rvc::DustDetector dust(map);
    rvc::RvcController ctrl(map, obs, dust);

    ctrl.handleSurroundedObstacle();
    const rvc::RvcSnapshot snap = ctrl.snapshot();
    EXPECT_EQ(snap.position, (rvc::Position{3, 3}));
    EXPECT_TRUE(snap.cleaningActive);
}

// FR-004, UR-001 — surrounded turn left fallback (SD-UC-004-S02)
TEST(RvcControllerTest, HandleSurroundedTurnLeft_FR004) {
    auto map = makeMap(6, 6, {2, 2}, rvc::Direction::North);
    map.setObstacles({{2, 1}, {1, 2}, {3, 2}, {3, 3}});
    rvc::ObstacleDetector obs(map);
    rvc::DustDetector dust(map);
    rvc::RvcController ctrl(map, obs, dust);

    ctrl.handleSurroundedObstacle();
    EXPECT_EQ(ctrl.snapshot().heading, rvc::Direction::West);
}

// FR-005, UR-003, NFR-004 — handleDustDetected (SD-UC-005-S01)
TEST(RvcControllerTest, HandleDustDetected_FR005) {
    auto map = makeMap(6, 6, {2, 3}, rvc::Direction::North);
    map.setDust({{2, 3}});
    rvc::ObstacleDetector obs(map);
    rvc::DustDetector dust(map);
    rvc::RvcController ctrl(map, obs, dust);

    ctrl.handleDustDetected();
    const rvc::RvcSnapshot snap = ctrl.snapshot();
    EXPECT_EQ(snap.level, rvc::OutputLevel::Boosted);
    EXPECT_EQ(snap.boostRemainingSec, 3);
}

// NFR-003 — IDustDetector abstraction
TEST(RvcControllerTest, DustDetectorInterface_NFR003) {
    auto map = makeMap(4, 4, {1, 1}, rvc::Direction::East);
    map.setDust({{1, 1}});
    rvc::DustDetector dust(map);
    EXPECT_TRUE(dust.isDustPresent());
}

// §0.4 — cleaning stops during obstacle maneuver
TEST(RvcControllerTest, CleaningStopsDuringManeuver_S04) {
    auto map = makeMap(6, 6, {2, 3}, rvc::Direction::North);
    map.setObstacles({{2, 2}});
    rvc::ObstacleDetector obs(map);
    rvc::DustDetector dust(map);
    rvc::RvcController ctrl(map, obs, dust);
    ctrl.startAutomaticCleaning();
    ctrl.handleObstacleDetected();
    EXPECT_TRUE(ctrl.snapshot().cleaningActive);
}
