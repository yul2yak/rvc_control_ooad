#pragma once

#include "rvc/SimulationEngine.hpp"

#include <SFML/Graphics.hpp>
#include <string>

class SimulatorApp {
public:
    SimulatorApp();

    int run();

private:
    static const int kCellSize = 40;
    static const int kPanelWidth = 280;

    sf::RenderWindow window_;
    rvc::SimulationEngine engine_;
    rvc::MapScenario scenario_;
    std::string statusMessage_;
    int tickCount_ = 0;
    bool editMode_ = false;
    bool placingObstacle_ = true;

    void resetDefaultScenario();
    void handleEvent(const sf::Event& event);
    void handleMouseEdit(int gridX, int gridY);
    void render();
    void drawGrid();
    void drawEntities();
    void drawPanel();
    sf::Vector2i pixelToGrid(int px, int py) const;
    void loadScenarioFile(const std::string& path);
    void saveScenarioFile(const std::string& path);
    void runTicks(int n);
    void updateStatusFromSnapshot();
};
