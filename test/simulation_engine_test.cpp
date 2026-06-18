#include "rvc/SimulationEngine.hpp"

#include <gtest/gtest.h>

// FR-001–003 integration — auto start and forward path
TEST(SimulationEngineTest, AutoForwardPath_FR002) {
    rvc::MapScenario scenario;
    scenario.width = 6;
    scenario.height = 6;
    scenario.rvcStart = {1, 4};
    scenario.rvcHeading = rvc::Direction::North;
    scenario.ticks = 3;
    scenario.trigger = "start";
    scenario.expectedPath = {{1, 4}, {1, 3}, {1, 2}, {1, 1}};

    rvc::SimulationEngine engine;
    engine.loadScenario(scenario);
    engine.runTicks(scenario.ticks);
    EXPECT_TRUE(engine.pathMatchesExpected());
}

// FR-003 — obstacle trigger scenario
TEST(SimulationEngineTest, ObstacleTrigger_FR003) {
    rvc::MapScenario scenario;
    scenario.width = 6;
    scenario.height = 6;
    scenario.rvcStart = {2, 3};
    scenario.rvcHeading = rvc::Direction::North;
    scenario.obstacles = {{2, 2}};
    scenario.trigger = "obstacle";
    scenario.ticks = 1;

    rvc::SimulationEngine engine;
    engine.loadScenario(scenario);
    engine.runTicks(1);
    EXPECT_EQ(engine.controller().snapshot().heading, rvc::Direction::East);
}

// FR-004 — surrounded trigger
TEST(SimulationEngineTest, SurroundedTrigger_FR004) {
    rvc::MapScenario scenario;
    scenario.width = 6;
    scenario.height = 6;
    scenario.rvcStart = {2, 2};
    scenario.rvcHeading = rvc::Direction::North;
    scenario.obstacles = {{2, 1}, {1, 2}, {3, 2}};
    scenario.trigger = "surrounded";
    scenario.ticks = 1;

    rvc::SimulationEngine engine;
    engine.loadScenario(scenario);
    engine.runTicks(1);
    EXPECT_EQ(engine.map().rvcPosition(), (rvc::Position{3, 3}));
}

// FR-005 — dust trigger
TEST(SimulationEngineTest, DustTrigger_FR005) {
    rvc::MapScenario scenario;
    scenario.width = 5;
    scenario.height = 5;
    scenario.rvcStart = {2, 2};
    scenario.rvcHeading = rvc::Direction::North;
    scenario.dust = {{2, 2}};
    scenario.trigger = "dust";
    scenario.ticks = 1;

    rvc::SimulationEngine engine;
    engine.loadScenario(scenario);
    engine.runTicks(1);
    EXPECT_EQ(engine.controller().snapshot().level, rvc::OutputLevel::Boosted);
}

// ST-025 regression — active session + surrounded L/R/F during auto ticks (sim edit bug)
TEST(SimulationEngineTest, ReloadLayoutKeepsSession_SurroundedAutoTick) {
    rvc::MapScenario scenario;
    scenario.width = 10;
    scenario.height = 10;
    scenario.rvcStart = {5, 7};
    scenario.rvcHeading = rvc::Direction::North;
    scenario.trigger = "start";

    rvc::SimulationEngine engine;
    engine.loadScenario(scenario);
    engine.tickOnce();
    EXPECT_EQ(engine.map().rvcPosition(), (rvc::Position{5, 6}));

    scenario.obstacles = {{5, 5}, {4, 6}, {6, 6}};
    engine.reloadLayout(scenario);
    EXPECT_TRUE(engine.map().isSurrounded());

    const rvc::Position posBefore = engine.map().rvcPosition();
    const rvc::Direction headingBefore = engine.map().rvcHeading();
    engine.tickOnce();
    const bool recovered = engine.map().rvcPosition() != posBefore ||
                           engine.map().rvcHeading() != headingBefore;
    EXPECT_TRUE(recovered);
}

// MapJson serialization
TEST(SimulationEngineTest, MapJsonRoundTrip) {
    rvc::MapScenario scenario;
    scenario.id = "TEST-001";
    scenario.name = "json test";
    scenario.width = 4;
    scenario.height = 4;
    scenario.rvcStart = {1, 1};
    scenario.rvcHeading = rvc::Direction::East;
    scenario.obstacles = {{2, 1}};
    scenario.dust = {{3, 1}};
    scenario.ticks = 2;
    scenario.trigger = "start";

    const std::string json = rvc::scenarioToJson(scenario);
    EXPECT_FALSE(json.empty());
    EXPECT_NE(json.find("TEST-001"), std::string::npos);
}
