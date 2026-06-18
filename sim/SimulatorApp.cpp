#include "SimulatorApp.hpp"

#include "rvc/MapJson.hpp"

#include <algorithm>
#include <sstream>

namespace {

sf::Color headingColor(rvc::Direction d) {
    switch (d) {
        case rvc::Direction::North:
            return sf::Color(80, 160, 255);
        case rvc::Direction::East:
            return sf::Color(80, 220, 120);
        case rvc::Direction::South:
            return sf::Color(255, 180, 80);
        case rvc::Direction::West:
            return sf::Color(200, 120, 255);
    }
    return sf::Color::White;
}

}  // namespace

SimulatorApp::SimulatorApp()
    : window_(sf::VideoMode(800, 600), "RVC SW Controller Simulator") {
    resetDefaultScenario();
}

void SimulatorApp::resetDefaultScenario() {
    scenario_.id = "SIM-DEFAULT";
    scenario_.name = "Default Map";
    scenario_.width = 12;
    scenario_.height = 10;
    scenario_.rvcStart = {2, 5};
    scenario_.rvcHeading = rvc::Direction::North;
    scenario_.obstacles.clear();
    scenario_.dust.clear();
    scenario_.ticks = 1;
    scenario_.trigger = "start";
    scenario_.expectedPath.clear();
    engine_.loadScenario(scenario_);
    tickCount_ = 0;
    statusMessage_ = "Ready — S:start  T:tick  R:run  L/S:load/save  E:edit  O/D:obstacle/dust";
}

sf::Vector2i SimulatorApp::pixelToGrid(int px, int py) const {
    if (px >= static_cast<int>(window_.getSize().x) - kPanelWidth) {
        return {-1, -1};
    }
    return {px / kCellSize, py / kCellSize};
}

void SimulatorApp::handleMouseEdit(int gridX, int gridY) {
    if (!editMode_ || gridX < 0 || gridY < 0) {
        return;
    }
    rvc::Position p{gridX, gridY};
    auto& obs = scenario_.obstacles;
    auto& dust = scenario_.dust;
    auto removeFrom = [](std::vector<rvc::Position>& list, const rvc::Position& pos) {
        list.erase(std::remove(list.begin(), list.end(), pos), list.end());
    };
    removeFrom(obs, p);
    removeFrom(dust, p);
    if (placingObstacle_) {
        obs.push_back(p);
    } else {
        dust.push_back(p);
    }
    engine_.loadScenario(scenario_);
}

void SimulatorApp::loadScenarioFile(const std::string& path) {
    rvc::MapScenario loaded;
    if (rvc::loadScenarioFromFile(path, loaded)) {
        scenario_ = loaded;
        engine_.loadScenario(scenario_);
        tickCount_ = 0;
        statusMessage_ = "Loaded: " + path;
    } else {
        statusMessage_ = "Load failed: " + path;
    }
}

void SimulatorApp::saveScenarioFile(const std::string& path) {
    scenario_.rvcStart = engine_.map().rvcPosition();
    scenario_.rvcHeading = engine_.map().rvcHeading();
    if (rvc::saveScenarioToFile(path, scenario_)) {
        statusMessage_ = "Saved: " + path;
    } else {
        statusMessage_ = "Save failed: " + path;
    }
}

void SimulatorApp::runTicks(int n) {
    engine_.runTicks(n);
    tickCount_ += n;
    updateStatusFromSnapshot();
}

void SimulatorApp::updateStatusFromSnapshot() {
    const rvc::RvcSnapshot snap = engine_.controller().snapshot();
    std::ostringstream ss;
    ss << "tick=" << tickCount_ << " pos=(" << snap.position.x << "," << snap.position.y << ")"
       << " heading=" << rvc::directionToString(snap.heading)
       << " clean=" << (snap.cleaningActive ? "Y" : "N")
       << " boost=" << (snap.level == rvc::OutputLevel::Boosted ? "Y" : "N");
    statusMessage_ = ss.str();
}

void SimulatorApp::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window_.close();
    }
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::S:
                engine_.triggerStart();
                ++tickCount_;
                updateStatusFromSnapshot();
                break;
            case sf::Keyboard::T:
                engine_.tickOnce();
                ++tickCount_;
                updateStatusFromSnapshot();
                break;
            case sf::Keyboard::R:
                runTicks(scenario_.ticks > 0 ? scenario_.ticks : 1);
                break;
            case sf::Keyboard::L:
                loadScenarioFile("System-Test/scenarios/ST-001.json");
                break;
            case sf::Keyboard::F5:
                saveScenarioFile("System-Test/scenarios/custom_map.json");
                break;
            case sf::Keyboard::E:
                editMode_ = !editMode_;
                statusMessage_ = editMode_ ? "Edit mode ON" : "Edit mode OFF";
                break;
            case sf::Keyboard::O:
                placingObstacle_ = true;
                break;
            case sf::Keyboard::D:
                placingObstacle_ = false;
                break;
            case sf::Keyboard::N:
                resetDefaultScenario();
                break;
            default:
                break;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2i g = pixelToGrid(event.mouseButton.x, event.mouseButton.y);
        handleMouseEdit(g.x, g.y);
    }
}

void SimulatorApp::drawGrid() {
    const int w = scenario_.width;
    const int h = scenario_.height;
    sf::RectangleShape cell(sf::Vector2f(static_cast<float>(kCellSize - 1),
                                         static_cast<float>(kCellSize - 1)));
    cell.setFillColor(sf::Color(40, 40, 48));
    cell.setOutlineColor(sf::Color(60, 60, 70));
    cell.setOutlineThickness(1.f);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            cell.setPosition(static_cast<float>(x * kCellSize),
                             static_cast<float>(y * kCellSize));
            window_.draw(cell);
        }
    }
}

void SimulatorApp::drawEntities() {
    sf::CircleShape dot(static_cast<float>(kCellSize) * 0.25f);
    dot.setOrigin(dot.getRadius(), dot.getRadius());

    for (const auto& o : scenario_.obstacles) {
        dot.setFillColor(sf::Color(200, 60, 60));
        dot.setPosition(static_cast<float>(o.x * kCellSize + kCellSize / 2),
                        static_cast<float>(o.y * kCellSize + kCellSize / 2));
        window_.draw(dot);
    }
    for (const auto& d : scenario_.dust) {
        dot.setFillColor(sf::Color(220, 200, 60));
        dot.setPosition(static_cast<float>(d.x * kCellSize + kCellSize / 2),
                        static_cast<float>(d.y * kCellSize + kCellSize / 2));
        window_.draw(dot);
    }

    const auto& path = engine_.map().path();
    sf::VertexArray trail(sf::LineStrip, path.size());
    for (std::size_t i = 0; i < path.size(); ++i) {
        trail[i].position = sf::Vector2f(static_cast<float>(path[i].x * kCellSize + kCellSize / 2),
                                         static_cast<float>(path[i].y * kCellSize + kCellSize / 2));
        trail[i].color = sf::Color(100, 200, 255, 180);
    }
    if (path.size() >= 2) {
        window_.draw(trail);
    }

    const rvc::RvcSnapshot snap = engine_.controller().snapshot();
    sf::RectangleShape rvc(sf::Vector2f(static_cast<float>(kCellSize - 8),
                                        static_cast<float>(kCellSize - 8)));
    rvc.setFillColor(headingColor(snap.heading));
    rvc.setPosition(static_cast<float>(snap.position.x * kCellSize + 4),
                    static_cast<float>(snap.position.y * kCellSize + 4));
    window_.draw(rvc);
}

void SimulatorApp::drawPanel() {
    const float panelX = static_cast<float>(window_.getSize().x - kPanelWidth);
    sf::RectangleShape panel(sf::Vector2f(static_cast<float>(kPanelWidth),
                                            static_cast<float>(window_.getSize().y)));
    panel.setPosition(panelX, 0.f);
    panel.setFillColor(sf::Color(28, 28, 36));
    window_.draw(panel);
}

void SimulatorApp::render() {
    window_.clear(sf::Color(24, 24, 30));
    drawGrid();
    drawEntities();
    drawPanel();
    window_.display();
}

int SimulatorApp::run() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            handleEvent(event);
        }
        render();
    }
    return 0;
}
