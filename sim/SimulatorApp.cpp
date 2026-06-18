#include "SimulatorApp.hpp"

#include "rvc/MapJson.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

const float SimulatorApp::kAutoTickIntervalSec = 0.5f;

namespace {

const sf::Color kObstacleColor(220, 60, 60);
const sf::Color kDustColor(240, 210, 50);
const sf::Color kRvcBodyColor(50, 110, 230);
const sf::Color kRvcArrowColor(180, 220, 255);
const sf::Color kRvcBoostBodyColor(240, 130, 35);
const sf::Color kRvcBoostArrowColor(255, 230, 160);
const sf::Color kButtonFill(50, 55, 70);
const sf::Color kButtonActive(70, 120, 180);
const sf::Color kButtonHover(65, 75, 95);

constexpr float kPanelPad = 12.f;
constexpr float kLegendRow = 20.f;
constexpr float kHelpRow = 15.f;

sf::String toSfString(const std::string& utf8) {
    return sf::String::fromUtf8(utf8.begin(), utf8.end());
}

void setUtf8Text(sf::Text& text, const std::string& utf8) {
    text.setString(toSfString(utf8));
}

std::string movementKindLabel(rvc::MovementKind kind) {
    switch (kind) {
        case rvc::MovementKind::Forward:
            return "Forward";
        case rvc::MovementKind::Backward:
            return "Backward";
        case rvc::MovementKind::Turning:
            return "Turning";
        case rvc::MovementKind::Idle:
        default:
            return "Idle";
    }
}

std::string formatPositions(const std::vector<rvc::Position>& list) {
    if (list.empty()) {
        return "[]";
    }
    std::ostringstream ss;
    ss << "[";
    for (std::size_t i = 0; i < list.size(); ++i) {
        if (i > 0) {
            ss << ",";
        }
        ss << "(" << list[i].x << "," << list[i].y << ")";
    }
    ss << "]";
    return ss.str();
}

#ifdef _WIN32
bool copyUtf8ToClipboard(const std::string& utf8) {
    if (utf8.empty()) {
        return false;
    }
    const int wlen =
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), nullptr, 0);
    if (wlen <= 0) {
        return false;
    }
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>((wlen + 1) * sizeof(wchar_t)));
    if (!mem) {
        return false;
    }
    auto* wide = static_cast<wchar_t*>(GlobalLock(mem));
    if (!wide) {
        GlobalFree(mem);
        return false;
    }
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), wide, wlen);
    wide[wlen] = L'\0';
    GlobalUnlock(mem);
    if (!OpenClipboard(nullptr)) {
        GlobalFree(mem);
        return false;
    }
    EmptyClipboard();
    const bool ok = SetClipboardData(CF_UNICODETEXT, mem) != nullptr;
    CloseClipboard();
    if (!ok) {
        GlobalFree(mem);
    }
    return ok;
}
#endif

}  // namespace

SimulatorApp::SimulatorApp()
    : window_(sf::VideoMode(920, 680), "RVC SW Controller Simulator") {
    loadFont();
    loadScenarioManifest();
    resetDefaultScenario();
    relayoutPanel();
    refreshSelectedScenarioLabel();
    applyWindowTitle();
}

const char* SimulatorApp::tr(const char* ko, const char* en) const {
    return englishUi_ ? en : ko;
}

void SimulatorApp::applyWindowTitle() {
    window_.setTitle(toSfString(tr("RVC SW Controller 시뮬레이터", "RVC SW Controller Simulator")));
}

void SimulatorApp::toggleLanguage() {
    englishUi_ = !englishUi_;
    applyWindowTitle();
    relayoutPanel();
    updateStatusFromSnapshot();
}

void SimulatorApp::relayoutPanel() {
    const float x = panelX() + kPanelPad;
    const float w = static_cast<float>(kPanelWidth) - kPanelPad * 2.f;

    legendStartY_ = kPanelPad;
    const float legendHeight = 22.f + kLegendRow * 3.f;
    const float langY = legendStartY_ + legendHeight + 6.f;
    langButtonBounds_ = sf::FloatRect(x, langY, w, 24.f);
    helpStartY_ = langY + 24.f + 8.f;

    const float helpLines = 7.f;
    const float helpHeight = 18.f + helpLines * kHelpRow;
    scenarioStartY_ = helpStartY_ + helpHeight + 10.f;
    buttonsStartY_ = scenarioStartY_ + 38.f;
    statusStartY_ = static_cast<float>(window_.getSize().y) - 64.f;
    layoutButtons(buttonsStartY_);
}

std::string SimulatorApp::buttonLabel(SimAction id) const {
    switch (id) {
        case SimAction::Start:
            return tr("시작 (S)", "Start (S)");
        case SimAction::Tick:
            return tr("틱 x1 (T)", "Tick x1 (T)");
        case SimAction::Run:
            return tr("N틱 실행 (R)", "Run N ticks (R)");
        case SimAction::AutoPlay:
            return tr("자동 0.5초 (A)", "Auto 0.5s (A)");
        case SimAction::Edit:
            return tr("맵 편집 (E)", "Edit map (E)");
        case SimAction::Obstacle:
            return tr("장애물 (O)", "Obstacle (O)");
        case SimAction::Dust:
            return tr("먼지 (D)", "Dust (D)");
        case SimAction::ScenarioPrev:
            return tr("이전 ([)", "Prev ([)");
        case SimAction::ScenarioNext:
            return tr("다음 (])", "Next (])");
        case SimAction::Load:
            return tr("시나리오 로드 (L)", "Load selected (L)");
        case SimAction::Save:
            return tr("맵 저장 (F5)", "Save map (F5)");
        case SimAction::Reset:
            return tr("맵 리셋 (N)", "Reset map (N)");
        default:
            return "";
    }
}

bool SimulatorApp::loadFont() {
    const char* candidates[] = {
        "C:/Windows/Fonts/malgun.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
    };
    for (const char* path : candidates) {
        if (font_.loadFromFile(path)) {
            return true;
        }
    }
    return false;
}

void SimulatorApp::loadScenarioManifest() {
    scenarioFiles_.clear();
    std::ifstream in("System-Test/scenarios/manifest.txt");
    if (!in) {
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        scenarioFiles_.push_back(line);
    }
    scenarioIndex_ = 0;
}

void SimulatorApp::refreshSelectedScenarioLabel() {
    if (scenarioFiles_.empty()) {
        selectedScenarioLabel_ = tr("(시나리오 없음)", "(no scenarios)");
        return;
    }
    if (scenarioIndex_ < 0) {
        scenarioIndex_ = 0;
    }
    if (scenarioIndex_ >= static_cast<int>(scenarioFiles_.size())) {
        scenarioIndex_ = static_cast<int>(scenarioFiles_.size()) - 1;
    }
    rvc::MapScenario meta;
    const std::string& path = scenarioFiles_[static_cast<std::size_t>(scenarioIndex_)];
    if (rvc::loadScenarioFromFile(path, meta)) {
        selectedScenarioLabel_ = meta.id + " — " + meta.name;
    } else {
        selectedScenarioLabel_ = path;
    }
}

void SimulatorApp::selectScenarioDelta(int delta) {
    if (scenarioFiles_.empty()) {
        return;
    }
    scenarioIndex_ = (scenarioIndex_ + delta) % static_cast<int>(scenarioFiles_.size());
    if (scenarioIndex_ < 0) {
        scenarioIndex_ += static_cast<int>(scenarioFiles_.size());
    }
    refreshSelectedScenarioLabel();
}

void SimulatorApp::loadSelectedScenario() {
    if (scenarioFiles_.empty()) {
        statusMessage_ = tr("manifest에 시나리오 없음", "No scenarios in manifest");
        return;
    }
    loadScenarioFile(scenarioFiles_[static_cast<std::size_t>(scenarioIndex_)]);
    refreshSelectedScenarioLabel();
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
    mapDebugLabel_ = "SIM-DEFAULT";
    loadedScenarioPath_.clear();
    engine_.loadScenario(scenario_);
    tickCount_ = 0;
    autoPlayMode_ = false;
    editMode_ = false;
    clearDebugLog();
    appendDebugHeader();
    updateStatusFromSnapshot();
}

float SimulatorApp::panelX() const {
    return static_cast<float>(window_.getSize().x - kPanelWidth);
}

void SimulatorApp::layoutButtons(float startY) {
    buttons_.clear();
    const float x = panelX() + kPanelPad;
    float y = startY;
    const float w = static_cast<float>(kPanelWidth) - kPanelPad * 2.f;
    const float h = 24.f;
    const float gap = 4.f;
    const float halfW = (w - gap) / 2.f;

    auto add = [&](SimAction id, bool toggle = false) {
        buttons_.push_back({sf::FloatRect(x, y, w, h), buttonLabel(id), id, toggle, false});
        y += h + gap;
    };
    auto addHalf = [&](SimAction id, float bx) {
        buttons_.push_back({sf::FloatRect(bx, y, halfW, h), buttonLabel(id), id, false, false});
    };

    add(SimAction::Start);
    add(SimAction::Tick);
    add(SimAction::Run);
    add(SimAction::AutoPlay, true);
    add(SimAction::Edit, true);
    add(SimAction::Obstacle, true);
    add(SimAction::Dust, true);
    addHalf(SimAction::ScenarioPrev, x);
    addHalf(SimAction::ScenarioNext, x + halfW + gap);
    y += h + gap;
    add(SimAction::Load);
    add(SimAction::Save);
    add(SimAction::Reset);
}

sf::Vector2i SimulatorApp::pixelToGrid(int px, int py) const {
    if (!isInGridArea(px, py)) {
        return {-1, -1};
    }
    return {px / kCellSize, py / kCellSize};
}

bool SimulatorApp::isInGridArea(int px, int py) const {
    return px >= 0 && px < static_cast<int>(panelX()) && py >= 0 &&
           py < static_cast<int>(window_.getSize().y);
}

SimulatorApp::SimAction SimulatorApp::hitTestButton(int px, int py) const {
    if (px < static_cast<int>(panelX())) {
        return SimAction::None;
    }
    const sf::Vector2f p(static_cast<float>(px), static_cast<float>(py));
    if (langButtonBounds_.contains(p)) {
        return SimAction::ToggleLanguage;
    }
    for (const auto& btn : buttons_) {
        if (btn.bounds.contains(p)) {
            return btn.id;
        }
    }
    return SimAction::None;
}

void SimulatorApp::handleGridClick(int gridX, int gridY) {
    if (!editMode_ || gridX < 0 || gridY < 0) {
        return;
    }
    if (gridX >= scenario_.width || gridY >= scenario_.height) {
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
    scenario_.rvcStart = engine_.map().rvcPosition();
    scenario_.rvcHeading = engine_.map().rvcHeading();
    mapDebugLabel_ = "custom";
    engine_.reloadLayout(scenario_);
}

void SimulatorApp::loadScenarioFile(const std::string& path) {
    rvc::MapScenario loaded;
    if (rvc::loadScenarioFromFile(path, loaded)) {
        scenario_ = loaded;
        mapDebugLabel_ = loaded.id;
        loadedScenarioPath_ = path;
        engine_.loadScenario(scenario_);
        tickCount_ = 0;
        clearDebugLog();
        appendDebugHeader();
        statusMessage_ = std::string(tr("로드: ", "Loaded: ")) + path;
    } else {
        statusMessage_ = std::string(tr("로드 실패: ", "Load failed: ")) + path;
    }
}

void SimulatorApp::saveScenarioFile(const std::string& path) {
    scenario_.rvcStart = engine_.map().rvcPosition();
    scenario_.rvcHeading = engine_.map().rvcHeading();
    if (rvc::saveScenarioToFile(path, scenario_)) {
        mapDebugLabel_ = "custom";
        loadedScenarioPath_ = path;
        statusMessage_ = std::string(tr("저장: ", "Saved: ")) + path;
    } else {
        statusMessage_ = std::string(tr("저장 실패: ", "Save failed: ")) + path;
    }
}

void SimulatorApp::clearDebugLog() {
    debugLog_.clear();
}

void SimulatorApp::appendDebugHeader() {
    const rvc::RvcSnapshot snap = engine_.controller().snapshot();
    auto pushLine = [this](const std::string& line) {
        debugLog_.push_back(line);
        std::cout << line << std::endl;
    };

    pushLine("=== RVC debug log ===");

    std::ostringstream ss;
    ss << "map=" << mapDebugLabel_;
    if (!scenario_.name.empty() && mapDebugLabel_ != "custom" &&
        mapDebugLabel_ != "SIM-DEFAULT") {
        ss << " (" << scenario_.name << ")";
    }
    pushLine(ss.str());

    if (!loadedScenarioPath_.empty()) {
        pushLine(std::string("file=") + loadedScenarioPath_);
    }

    ss.str("");
    ss << "trigger=" << scenario_.trigger << " ticks=" << scenario_.ticks << " size="
       << scenario_.width << "x" << scenario_.height;
    pushLine(ss.str());

    ss.str("");
    ss << "start pos=(" << snap.position.x << "," << snap.position.y << ") heading="
       << rvc::directionToString(snap.heading) << " session="
       << (engine_.isSessionActive() ? 1 : 0);
    pushLine(ss.str());

    pushLine(std::string("obstacles=") + formatPositions(scenario_.obstacles));
    pushLine(std::string("dust=") + formatPositions(scenario_.dust));

    if (!scenario_.expectedPath.empty()) {
        pushLine(std::string("expectedPath=") + formatPositions(scenario_.expectedPath));
    }

    pushLine("--- ticks (copy below) ---");
}

void SimulatorApp::appendTickLogLine() {
    const rvc::RvcSnapshot snap = engine_.controller().snapshot();
    const rvc::MapModel& map = engine_.map();
    const char* clean = snap.cleaningActive ? "1" : "0";
    const char* mop = snap.moppingActive ? "1" : "0";
    const char* boost = snap.level == rvc::OutputLevel::Boosted ? "1" : "0";

    std::ostringstream ss;
    ss << "tick=" << tickCount_ << " move=" << movementKindLabel(snap.movementKind) << " pos=("
       << snap.position.x << "," << snap.position.y << ")"
       << " dir=" << rvc::directionToString(snap.heading) << " clean=" << clean << " mop=" << mop
       << " boost=" << boost;
    if (snap.boostRemainingSec > 0) {
        ss << "(" << snap.boostRemainingSec << "s)";
    }
    ss << " session=" << (engine_.isSessionActive() ? 1 : 0)
       << " maneuver=" << (engine_.controller().maneuverInProgress() ? 1 : 0)
       << " front_blk=" << (map.isFrontBlocked() ? 1 : 0)
       << " surrounded=" << (engine_.obstacleDetector().isSurrounded() ? 1 : 0);

    const auto& path = map.path();
    if (!path.empty()) {
        ss << " path_len=" << path.size();
    }

    debugLog_.push_back(ss.str());
    std::cout << debugLog_.back() << std::endl;
}

std::string SimulatorApp::buildDebugLogText() const {
    std::ostringstream ss;
    for (std::size_t i = 0; i < debugLog_.size(); ++i) {
        if (i > 0) {
            ss << '\n';
        }
        ss << debugLog_[i];
    }
    return ss.str();
}

void SimulatorApp::copyDebugLogToClipboard() {
    const std::string text = buildDebugLogText();
    if (text.empty()) {
        statusMessage_ = tr("복사할 로그 없음", "No log to copy");
        return;
    }
#ifdef _WIN32
    if (copyUtf8ToClipboard(text)) {
        statusMessage_ = tr("디버그 로그 클립보드 복사됨 (", "Debug log copied (") +
                           std::to_string(debugLog_.size()) + tr("줄)", " lines)");
    } else {
        statusMessage_ = tr("클립보드 복사 실패", "Clipboard copy failed");
    }
#else
    statusMessage_ = tr("클립보드 미지원 — F6으로 저장", "Clipboard unsupported — use F6 export");
#endif
}

void SimulatorApp::exportDebugLogToFile() {
    const std::string path = "System-Test/scenarios/debug_log.txt";
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        statusMessage_ = std::string(tr("저장 실패: ", "Save failed: ")) + path;
        return;
    }
    const std::string text = buildDebugLogText();
    out.write(text.data(), static_cast<std::streamsize>(text.size()));
    out.put('\n');
    statusMessage_ = std::string(tr("디버그 로그 저장: ", "Debug log saved: ")) + path;
}

void SimulatorApp::doSingleTick() {
    engine_.tickOnce();
    ++tickCount_;
    appendTickLogLine();
    updateStatusFromSnapshot();
}

void SimulatorApp::runTicks(int n) {
    const int count = n > 0 ? n : 1;
    for (int i = 0; i < count; ++i) {
        doSingleTick();
    }
}

void SimulatorApp::updateStatusFromSnapshot() {
    scenario_.dust = engine_.map().dustCells();
    const rvc::RvcSnapshot snap = engine_.controller().snapshot();
    const char* onOff = tr("켜짐", "ON");
    const char* offOff = tr("꺼짐", "OFF");
    std::ostringstream ss;
    ss << tr("틱=", "tick=") << tickCount_ << "  pos=(" << snap.position.x << ","
       << snap.position.y << ")"
       << "  dir=" << rvc::directionToString(snap.heading)
       << "  clean=" << (snap.cleaningActive ? onOff : offOff)
       << "  boost=" << (snap.level == rvc::OutputLevel::Boosted ? onOff : offOff);
    if (autoPlayMode_) {
        ss << tr("  [자동 0.5초]", "  [AUTO 0.5s]");
    }
    if (editMode_) {
        ss << tr("  [편집:", "  [EDIT:") << (placingObstacle_ ? tr("장애물", "obstacle")
                                                     : tr("먼지", "dust"))
           << "]";
    }
    ss << tr("  | 로그 ", "  | log ") << debugLog_.size() << tr("줄 C복사 F6저장",
                                                                 " lines C=copy F6=save");
    statusMessage_ = ss.str();
}

void SimulatorApp::activateButton(SimulatorApp::SimAction id) {
    switch (id) {
        case SimAction::Start:
            if (!engine_.isSessionActive()) {
                doSingleTick();
            }
            break;
        case SimAction::Tick:
            doSingleTick();
            break;
        case SimAction::Run:
            runTicks(scenario_.ticks > 0 ? scenario_.ticks : 1);
            break;
        case SimAction::AutoPlay:
            autoPlayMode_ = !autoPlayMode_;
            autoPlayClock_.restart();
            updateStatusFromSnapshot();
            break;
        case SimAction::Edit:
            editMode_ = !editMode_;
            updateStatusFromSnapshot();
            break;
        case SimAction::Obstacle:
            placingObstacle_ = true;
            editMode_ = true;
            updateStatusFromSnapshot();
            break;
        case SimAction::Dust:
            placingObstacle_ = false;
            editMode_ = true;
            updateStatusFromSnapshot();
            break;
        case SimAction::Load:
            loadSelectedScenario();
            break;
        case SimAction::ScenarioPrev:
            selectScenarioDelta(-1);
            break;
        case SimAction::ScenarioNext:
            selectScenarioDelta(+1);
            break;
        case SimAction::Save:
            saveScenarioFile("System-Test/scenarios/custom_map.json");
            break;
        case SimAction::Reset:
            resetDefaultScenario();
            break;
        case SimAction::CopyLog:
            copyDebugLogToClipboard();
            break;
        case SimAction::ExportLog:
            exportDebugLogToFile();
            break;
        case SimAction::ToggleLanguage:
            toggleLanguage();
            break;
        default:
            break;
    }
}

void SimulatorApp::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window_.close();
    }
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::S:
                activateButton(SimAction::Start);
                break;
            case sf::Keyboard::T:
                activateButton(SimAction::Tick);
                break;
            case sf::Keyboard::R:
                activateButton(SimAction::Run);
                break;
            case sf::Keyboard::A:
                activateButton(SimAction::AutoPlay);
                break;
            case sf::Keyboard::L:
                activateButton(SimAction::Load);
                break;
            case sf::Keyboard::LBracket:
                activateButton(SimAction::ScenarioPrev);
                break;
            case sf::Keyboard::RBracket:
                activateButton(SimAction::ScenarioNext);
                break;
            case sf::Keyboard::F5:
                activateButton(SimAction::Save);
                break;
            case sf::Keyboard::E:
                activateButton(SimAction::Edit);
                break;
            case sf::Keyboard::O:
                activateButton(SimAction::Obstacle);
                break;
            case sf::Keyboard::D:
                activateButton(SimAction::Dust);
                break;
            case sf::Keyboard::N:
                activateButton(SimAction::Reset);
                break;
            case sf::Keyboard::C:
                activateButton(SimAction::CopyLog);
                break;
            case sf::Keyboard::F6:
                activateButton(SimAction::ExportLog);
                break;
            case sf::Keyboard::H:
                toggleLanguage();
                break;
            default:
                break;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        const int mx = event.mouseButton.x;
        const int my = event.mouseButton.y;
        const SimulatorApp::SimAction btn = hitTestButton(mx, my);
        if (btn != SimAction::None) {
            activateButton(btn);
            return;
        }
        const sf::Vector2i g = pixelToGrid(mx, my);
        handleGridClick(g.x, g.y);
    }
}

void SimulatorApp::update() {
    if (!autoPlayMode_) {
        return;
    }
    if (autoPlayClock_.getElapsedTime().asSeconds() >= kAutoTickIntervalSec) {
        autoPlayClock_.restart();
        doSingleTick();
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

void SimulatorApp::drawRvc(const rvc::RvcSnapshot& snap) {
    const float cx = static_cast<float>(snap.position.x * kCellSize + kCellSize / 2);
    const float cy = static_cast<float>(snap.position.y * kCellSize + kCellSize / 2);
    const float pad = 2.f;
    const float radius = (static_cast<float>(kCellSize) - pad * 2.f) / 2.f;

    const bool boosted = snap.level == rvc::OutputLevel::Boosted;
    const sf::Color bodyColor = boosted ? kRvcBoostBodyColor : kRvcBodyColor;
    const sf::Color outlineColor =
        boosted ? sf::Color(180, 80, 10) : sf::Color(30, 70, 160);
    const sf::Color arrowColor = boosted ? kRvcBoostArrowColor : kRvcArrowColor;

    sf::CircleShape body(radius);
    body.setFillColor(bodyColor);
    body.setOutlineColor(outlineColor);
    body.setOutlineThickness(2.f);
    body.setOrigin(radius, radius);
    body.setPosition(cx, cy);
    window_.draw(body);

    sf::ConvexShape arrow(3);
    arrow.setFillColor(arrowColor);
    arrow.setOutlineColor(boosted ? sf::Color(255, 255, 255, 180) : sf::Color(255, 255, 255, 120));
    arrow.setOutlineThickness(1.f);
    const float s = static_cast<float>(kCellSize) * 0.16f;
    switch (snap.heading) {
        case rvc::Direction::North:
            arrow.setPoint(0, sf::Vector2f(0.f, -s * 1.35f));
            arrow.setPoint(1, sf::Vector2f(-s * 0.75f, s * 0.35f));
            arrow.setPoint(2, sf::Vector2f(s * 0.75f, s * 0.35f));
            break;
        case rvc::Direction::East:
            arrow.setPoint(0, sf::Vector2f(s * 1.35f, 0.f));
            arrow.setPoint(1, sf::Vector2f(-s * 0.35f, -s * 0.75f));
            arrow.setPoint(2, sf::Vector2f(-s * 0.35f, s * 0.75f));
            break;
        case rvc::Direction::South:
            arrow.setPoint(0, sf::Vector2f(0.f, s * 1.35f));
            arrow.setPoint(1, sf::Vector2f(-s * 0.75f, -s * 0.35f));
            arrow.setPoint(2, sf::Vector2f(s * 0.75f, -s * 0.35f));
            break;
        case rvc::Direction::West:
            arrow.setPoint(0, sf::Vector2f(-s * 1.35f, 0.f));
            arrow.setPoint(1, sf::Vector2f(s * 0.35f, -s * 0.75f));
            arrow.setPoint(2, sf::Vector2f(s * 0.35f, s * 0.75f));
            break;
    }
    arrow.setPosition(cx, cy);
    window_.draw(arrow);
}

void SimulatorApp::drawEntities() {
    const float pad = 4.f;
    const float obsSize = static_cast<float>(kCellSize) - pad * 2.f;

    for (const auto& o : scenario_.obstacles) {
        sf::RectangleShape obs(sf::Vector2f(obsSize, obsSize));
        obs.setFillColor(kObstacleColor);
        obs.setOutlineColor(sf::Color(140, 30, 30));
        obs.setOutlineThickness(1.5f);
        obs.setPosition(static_cast<float>(o.x * kCellSize + pad),
                        static_cast<float>(o.y * kCellSize + pad));
        window_.draw(obs);
    }

    const float dustRadius = static_cast<float>(kCellSize) * 0.22f;
    for (const auto& d : engine_.map().dustCells()) {
        sf::CircleShape dust(dustRadius);
        dust.setFillColor(kDustColor);
        dust.setOutlineColor(sf::Color(180, 140, 20));
        dust.setOutlineThickness(1.5f);
        dust.setOrigin(dustRadius, dustRadius);
        dust.setPosition(static_cast<float>(d.x * kCellSize + kCellSize / 2),
                         static_cast<float>(d.y * kCellSize + kCellSize / 2));
        window_.draw(dust);
    }

    const auto& path = engine_.map().path();
    if (path.size() >= 2) {
        sf::VertexArray trail(sf::LineStrip, path.size());
        for (std::size_t i = 0; i < path.size(); ++i) {
            trail[i].position =
                sf::Vector2f(static_cast<float>(path[i].x * kCellSize + kCellSize / 2),
                             static_cast<float>(path[i].y * kCellSize + kCellSize / 2));
            trail[i].color = sf::Color(120, 200, 255, 160);
        }
        window_.draw(trail);
    }

    drawRvc(engine_.controller().snapshot());
}

void SimulatorApp::drawTextLine(const std::string& text, float x, float y, unsigned int size,
                                const sf::Color& color) {
    sf::Text t;
    t.setFont(font_);
    setUtf8Text(t, text);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setPosition(x, y);
    window_.draw(t);
}

void SimulatorApp::drawLanguageToggle() {
    const sf::Vector2i mouse = sf::Mouse::getPosition(window_);
    sf::RectangleShape rect(
        sf::Vector2f(langButtonBounds_.width, langButtonBounds_.height));
    rect.setPosition(langButtonBounds_.left, langButtonBounds_.top);
    const bool hover = langButtonBounds_.contains(static_cast<float>(mouse.x),
                                                  static_cast<float>(mouse.y));
    rect.setFillColor(hover ? kButtonHover : kButtonFill);
    rect.setOutlineColor(sf::Color(90, 95, 110));
    rect.setOutlineThickness(1.f);
    window_.draw(rect);

    const std::string label =
        englishUi_ ? tr("한국어로 전환 (H)", "Switch to Korean (H)")
                   : tr("English (H)", "Switch to English (H)");
    sf::Text t;
    t.setFont(font_);
    setUtf8Text(t, label);
    t.setCharacterSize(11);
    t.setFillColor(sf::Color(230, 230, 240));
    const sf::FloatRect tb = t.getLocalBounds();
    t.setPosition(langButtonBounds_.left + (langButtonBounds_.width - tb.width) / 2.f - tb.left,
                  langButtonBounds_.top + (langButtonBounds_.height - tb.height) / 2.f - tb.top -
                      2.f);
    window_.draw(t);
}

void SimulatorApp::drawLegend() {
    const float x = panelX() + kPanelPad;
    float y = legendStartY_;
    drawTextLine(tr("범례", "Legend"), x, y, 14, sf::Color(220, 220, 230));
    y += 22.f;

    sf::CircleShape rvcDot(7.f);
    rvcDot.setFillColor(kRvcBodyColor);
    rvcDot.setOutlineColor(sf::Color(30, 70, 160));
    rvcDot.setOutlineThickness(1.f);
    rvcDot.setPosition(x, y + 1.f);
    window_.draw(rvcDot);

    sf::CircleShape boostDot(7.f);
    boostDot.setFillColor(kRvcBoostBodyColor);
    boostDot.setOutlineColor(sf::Color(180, 80, 10));
    boostDot.setOutlineThickness(1.f);
    boostDot.setPosition(x + 18.f, y + 1.f);
    window_.draw(boostDot);

    drawTextLine(tr("청소기 (파랑 / 부스터 주황)", "RVC (blue / boost orange)"), x + 40.f, y, 12,
                 sf::Color(190, 190, 200));
    y += kLegendRow;

    sf::RectangleShape obsBox(sf::Vector2f(14.f, 14.f));
    obsBox.setFillColor(kObstacleColor);
    obsBox.setPosition(x, y + 2.f);
    window_.draw(obsBox);
    drawTextLine(tr("장애물 (빨간 사각형)", "Obstacle (red square)"), x + 22.f, y, 12,
                 sf::Color(190, 190, 200));
    y += kLegendRow;

    sf::CircleShape dustDot(7.f);
    dustDot.setFillColor(kDustColor);
    dustDot.setPosition(x, y + 2.f);
    window_.draw(dustDot);
    drawTextLine(tr("먼지 (노란 원)", "Dust (yellow circle)"), x + 22.f, y, 12,
                 sf::Color(190, 190, 200));
}

void SimulatorApp::drawHelpText() {
    const float x = panelX() + kPanelPad;
    float y = helpStartY_;
    drawTextLine(tr("조작 (키보드 / 클릭)", "Controls (keyboard / click)"), x, y, 13,
                 sf::Color(160, 200, 255));
    y += 18.f;
    drawTextLine(tr("S 시작  T 틱  R N틱 실행", "S Start  T Tick  R Run"), x, y, 11,
                 sf::Color(170, 170, 180));
    y += kHelpRow;
    drawTextLine(tr("A 자동 0.5초  E 편집", "A Auto 0.5s  E Edit"), x, y, 11,
                 sf::Color(170, 170, 180));
    y += kHelpRow;
    drawTextLine(tr("O 장애물  D 먼지", "O Obstacle  D Dust"), x, y, 11,
                 sf::Color(170, 170, 180));
    y += kHelpRow;
    drawTextLine(tr("[ 이전  ] 다음  L 로드", "[ Prev  ] Next  L Load"), x, y, 11,
                 sf::Color(170, 170, 180));
    y += kHelpRow;
    drawTextLine(tr("F5 저장  N 리셋  H 언어", "F5 Save  N Reset  H Lang"), x, y, 11,
                 sf::Color(170, 170, 180));
    y += kHelpRow;
    drawTextLine(tr("C 로그 복사  F6 로그 저장", "C Copy log  F6 Save log"), x, y, 11,
                 sf::Color(170, 170, 180));
    y += kHelpRow;
    drawTextLine(tr("편집: 그리드 셀 클릭", "Edit: click grid cell"), x, y, 11,
                 sf::Color(140, 140, 150));
}

void SimulatorApp::drawButtons() {
    const sf::Vector2i mouse = sf::Mouse::getPosition(window_);
    for (const auto& btn : buttons_) {
        sf::RectangleShape rect(sf::Vector2f(btn.bounds.width, btn.bounds.height));
        rect.setPosition(btn.bounds.left, btn.bounds.top);

        const bool active =
            (btn.id == SimAction::AutoPlay && autoPlayMode_) ||
            (btn.id == SimAction::Edit && editMode_) ||
            (btn.id == SimAction::Obstacle && editMode_ && placingObstacle_) ||
            (btn.id == SimAction::Dust && editMode_ && !placingObstacle_);

        const bool hover = btn.bounds.contains(static_cast<float>(mouse.x),
                                               static_cast<float>(mouse.y));
        if (active) {
            rect.setFillColor(kButtonActive);
        } else if (hover) {
            rect.setFillColor(kButtonHover);
        } else {
            rect.setFillColor(kButtonFill);
        }
        rect.setOutlineColor(sf::Color(90, 95, 110));
        rect.setOutlineThickness(1.f);
        window_.draw(rect);

        sf::Text label;
        label.setFont(font_);
        setUtf8Text(label, btn.label);
        label.setCharacterSize(12);
        label.setFillColor(sf::Color(230, 230, 240));
        const sf::FloatRect tb = label.getLocalBounds();
        label.setPosition(btn.bounds.left + (btn.bounds.width - tb.width) / 2.f - tb.left,
                          btn.bounds.top + (btn.bounds.height - tb.height) / 2.f - tb.top - 2.f);
        window_.draw(label);
    }
}

void SimulatorApp::drawPanel() {
    sf::RectangleShape panel(
        sf::Vector2f(static_cast<float>(kPanelWidth), static_cast<float>(window_.getSize().y)));
    panel.setPosition(panelX(), 0.f);
    panel.setFillColor(sf::Color(28, 28, 36));
    window_.draw(panel);

    drawLegend();
    drawLanguageToggle();
    drawHelpText();

    drawTextLine(tr("시나리오", "Scenario"), panelX() + kPanelPad, scenarioStartY_, 13,
                 sf::Color(160, 200, 255));
    drawTextLine(selectedScenarioLabel_, panelX() + kPanelPad, scenarioStartY_ + 18.f, 11,
                 sf::Color(200, 200, 210));

    drawButtons();

    drawTextLine(tr("상태", "Status"), panelX() + kPanelPad, statusStartY_, 13,
                 sf::Color(160, 200, 255));
    drawTextLine(statusMessage_, panelX() + kPanelPad, statusStartY_ + 20.f, 11,
                 sf::Color(200, 200, 210));
}

void SimulatorApp::render() {
    window_.clear(sf::Color(24, 24, 30));
    drawGrid();
    drawEntities();
    drawPanel();
    window_.display();
}

int SimulatorApp::run() {
    autoPlayClock_.restart();
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            handleEvent(event);
        }
        update();
        render();
    }
    return 0;
}
