#include "rvc/CleaningService.hpp"
#include "rvc/MapModel.hpp"
#include "rvc/MovementController.hpp"
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

// FR-002
TEST(MovementControllerTest, MoveForwardWithCleaning_FR002) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    rvc::CleaningService clean;
    rvc::ObstacleDetector obs(map);
    rvc::MovementController move(map, clean, obs);

    move.moveForwardWithCleaning();
    EXPECT_EQ(map.rvcPosition(), (rvc::Position{2, 1}));
    EXPECT_TRUE(clean.isCleaningActive());
}

// FR-004, section 0.4
TEST(MovementControllerTest, MoveBackward_FR004) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    rvc::CleaningService clean;
    clean.activateCleaningAndMopping();
    rvc::ObstacleDetector obs(map);
    rvc::MovementController move(map, clean, obs);

    move.moveBackward();
    EXPECT_EQ(map.rvcPosition(), (rvc::Position{2, 3}));
}

// FR-003, UR-001
TEST(MovementControllerTest, CanTurnRight_FR003_UR001) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    map.setObstacles({{3, 2}});
    rvc::CleaningService clean;
    rvc::ObstacleDetector obs(map);
    rvc::MovementController move(map, clean, obs);

    EXPECT_FALSE(move.canTurnRight());
}

// FR-003
TEST(MovementControllerTest, TurnRight_FR003) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    rvc::CleaningService clean;
    rvc::ObstacleDetector obs(map);
    rvc::MovementController move(map, clean, obs);

    move.turnRight();
    EXPECT_EQ(move.heading(), rvc::Direction::East);
}

// FR-003, UR-001
TEST(MovementControllerTest, TurnLeft_FR003) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    rvc::CleaningService clean;
    rvc::ObstacleDetector obs(map);
    rvc::MovementController move(map, clean, obs);

    move.turnLeft();
    EXPECT_EQ(move.heading(), rvc::Direction::West);
}

// FR-003, section 0.4
TEST(MovementControllerTest, ResumeForwardWithCleaning_FR003) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::North);
    rvc::CleaningService clean;
    clean.stopCleaning();
    rvc::ObstacleDetector obs(map);
    rvc::MovementController move(map, clean, obs);

    move.resumeForwardWithCleaning();
    EXPECT_EQ(map.rvcPosition(), (rvc::Position{2, 1}));
    EXPECT_TRUE(clean.isCleaningActive());
}

// NFR-003
TEST(MovementControllerTest, ObstacleDetectorInterface_NFR003) {
    auto map = makeMap(5, 5, {2, 2}, rvc::Direction::East);
    rvc::ObstacleDetector obs(map);
    EXPECT_TRUE(obs.isRightTurnFeasible());
    map.setObstacles({{2, 3}});
    EXPECT_FALSE(obs.isRightTurnFeasible());
}
