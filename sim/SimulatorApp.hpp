#pragma once

#include "rvc/SimulationEngine.hpp"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class SimulatorApp {
public:
    SimulatorApp();

    int run();

private:
    enum class SimAction {
        None,
        Start,
        Tick,
        Run,
        AutoPlay,
        Edit,
        Obstacle,
        Dust,
        Load,
        ScenarioPrev,
        ScenarioNext,
        Save,
        Reset,
        ToggleLanguage,
        CopyLog,
        ExportLog
    };

    struct UiButton {
        sf::FloatRect bounds;
        std::string label;
        SimAction id = SimAction::None;
        bool toggle = false;
        bool active = false;
    };

    static const int kCellSize = 40;
    static const int kPanelWidth = 300;
    static const float kAutoTickIntervalSec;

    sf::RenderWindow window_;
    sf::Font font_;
    rvc::SimulationEngine engine_;
    rvc::MapScenario scenario_;
    std::string statusMessage_;
    int tickCount_ = 0;
    bool editMode_ = false;
    bool placingObstacle_ = true;
    bool autoPlayMode_ = false;
    sf::Clock autoPlayClock_;
    std::vector<UiButton> buttons_;
    std::vector<std::string> scenarioFiles_;
    int scenarioIndex_ = 0;
    std::string selectedScenarioLabel_;
    std::string mapDebugLabel_;
    std::string loadedScenarioPath_;
    std::vector<std::string> debugLog_;
    bool englishUi_ = false;
    sf::FloatRect langButtonBounds_;
    float legendStartY_ = 12.f;
    float helpStartY_ = 100.f;
    float scenarioStartY_ = 228.f;
    float buttonsStartY_ = 268.f;
    float statusStartY_ = 560.f;

    void resetDefaultScenario();
    void relayoutPanel();
    void layoutButtons(float startY);
    const char* tr(const char* ko, const char* en) const;
    std::string buttonLabel(SimAction id) const;
    void toggleLanguage();
    void applyWindowTitle();
    void loadScenarioManifest();
    void refreshSelectedScenarioLabel();
    void selectScenarioDelta(int delta);
    void loadSelectedScenario();
    void handleEvent(const sf::Event& event);
    void handleGridClick(int gridX, int gridY);
    void activateButton(SimAction id);
    SimAction hitTestButton(int px, int py) const;
    void update();
    void render();
    void drawGrid();
    void drawEntities();
    void drawRvc(const rvc::RvcSnapshot& snap);
    void drawPanel();
    void drawLegend();
    void drawLanguageToggle();
    void drawHelpText();
    void drawButtons();
    void drawTextLine(const std::string& text, float x, float y, unsigned int size,
                      const sf::Color& color);
    sf::Vector2i pixelToGrid(int px, int py) const;
    bool isInGridArea(int px, int py) const;
    float panelX() const;
    void loadScenarioFile(const std::string& path);
    void saveScenarioFile(const std::string& path);
    void runTicks(int n);
    void doSingleTick();
    void updateStatusFromSnapshot();
    void clearDebugLog();
    void appendDebugHeader();
    void appendTickLogLine();
    std::string buildDebugLogText() const;
    void copyDebugLogToClipboard();
    void exportDebugLogToFile();
    bool loadFont();
};
