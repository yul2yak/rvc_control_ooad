#include "rvc/GridSensorReader.hpp"
#include "rvc/MapModel.hpp"
#include "rvc/ObstacleDetector.hpp"

#include <gtest/gtest.h>

namespace {

rvc::MapModel makeMap(int w, int h, rvc::Position start, rvc::Direction heading) {
    rvc::MapModel map;
    map.setSize(w, h);
    map.setRvcStart(start, heading);
    return map;
}

}  // namespace

// FR-003 — right obstacle inferred via virtual-pose front sensor (no right sensor HW)
TEST(ObstacleDetectorTest, RightInferredViaVirtualFront_FR003) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    map.setObstacles({{3, 2}});
    rvc::GridSensorReader sensor(map);
    rvc::ObstacleDetector obs(sensor);

    EXPECT_FALSE(obs.isRightTurnFeasible());
}

// FR-004 — surrounded uses inferred right via virtual-pose front
TEST(ObstacleDetectorTest, SurroundedUsesInferredRight_FR004) {
    auto map = makeMap(6, 6, {2, 2}, rvc::Direction::North);
    map.setObstacles({{2, 1}, {1, 2}, {3, 2}});
    rvc::GridSensorReader sensor(map);
    rvc::ObstacleDetector obs(sensor);

    EXPECT_TRUE(obs.isSurrounded());
}

// NFR-003 — sensor reader exposes front, left, virtual-front only
TEST(ObstacleDetectorTest, SensorReaderOnlyFrontLeft_NFR003) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    rvc::GridSensorReader sensor(map);

    EXPECT_FALSE(sensor.isFrontBlocked());
    EXPECT_FALSE(sensor.isLeftBlocked());
    EXPECT_FALSE(sensor.isFrontBlockedAtHeading(rvc::Direction::East));
}

// FR-004 — wouldForwardLeadToSurrounded uses inferred surrounded check
TEST(ObstacleDetectorTest, WouldForwardLeadToSurrounded_FR004) {
    auto map = makeMap(6, 6, {2, 3}, rvc::Direction::North);
    map.setObstacles({{2, 1}, {1, 2}, {3, 2}});
    rvc::GridSensorReader sensor(map);
    rvc::ObstacleDetector obs(sensor);

    EXPECT_TRUE(obs.wouldForwardLeadToSurrounded());
}
