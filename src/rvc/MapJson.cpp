#include "rvc/MapJson.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace rvc {

namespace {

std::string readFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("cannot open file: " + path);
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

void writeFile(const std::string& path, const std::string& content) {
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("cannot write file: " + path);
    }
    out << content;
}

std::string trim(const std::string& s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string extractStringField(const std::string& json, const std::string& key) {
    const std::string needle = "\"" + key + "\"";
    auto pos = json.find(needle);
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos = json.find('"', pos);
    if (pos == std::string::npos) return "";
    auto end = json.find('"', pos + 1);
    return json.substr(pos + 1, end - pos - 1);
}

int extractIntField(const std::string& json, const std::string& key, int fallback) {
    const std::string needle = "\"" + key + "\"";
    auto pos = json.find(needle);
    if (pos == std::string::npos) return fallback;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return fallback;
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) ++pos;
    return std::stoi(json.substr(pos));
}

std::vector<Position> extractPointArray(const std::string& json, const std::string& key) {
    std::vector<Position> out;
    const std::string needle = "\"" + key + "\"";
    auto pos = json.find(needle);
    if (pos == std::string::npos) return out;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return out;
    auto end = json.find(']', pos);
    std::string block = json.substr(pos + 1, end - pos - 1);
    std::size_t i = 0;
    while (i < block.size()) {
        auto xPos = block.find("\"x\"", i);
        if (xPos == std::string::npos) break;
        auto yPos = block.find("\"y\"", xPos);
        if (yPos == std::string::npos) break;
        xPos = block.find(':', xPos);
        yPos = block.find(':', yPos);
        int x = std::stoi(block.substr(xPos + 1));
        int y = std::stoi(block.substr(yPos + 1));
        out.push_back({x, y});
        i = block.find('}', yPos);
        if (i == std::string::npos) break;
        ++i;
    }
    return out;
}

Position extractRvcPosition(const std::string& json) {
    auto pos = json.find("\"rvc\"");
    if (pos == std::string::npos) return {0, 0};
    auto end = json.find('}', pos);
    std::string block = json.substr(pos, end - pos);
    int x = extractIntField(block, "x", 0);
    int y = extractIntField(block, "y", 0);
    return {x, y};
}

Direction extractRvcHeading(const std::string& json) {
    auto pos = json.find("\"rvc\"");
    if (pos == std::string::npos) return Direction::North;
    auto end = json.find('}', pos);
    std::string block = json.substr(pos, end - pos);
    return directionFromString(extractStringField(block, "heading"));
}

}  // namespace

bool loadScenarioFromFile(const std::string& path, MapScenario& out) {
    try {
        const std::string json = readFile(path);
        out.id = extractStringField(json, "id");
        out.name = extractStringField(json, "name");
        out.width = extractIntField(json, "width", 10);
        out.height = extractIntField(json, "height", 10);
        out.rvcStart = extractRvcPosition(json);
        out.rvcHeading = extractRvcHeading(json);
        out.obstacles = extractPointArray(json, "obstacles");
        out.dust = extractPointArray(json, "dust");
        out.ticks = extractIntField(json, "ticks", 1);
        out.trigger = extractStringField(json, "trigger");
        if (out.trigger.empty()) out.trigger = "start";
        out.expectedPath = extractPointArray(json, "expectedPath");
        return true;
    } catch (...) {
        return false;
    }
}

std::string scenarioToJson(const MapScenario& scenario) {
    std::ostringstream ss;
    ss << "{\n";
    ss << "  \"id\": \"" << scenario.id << "\",\n";
    ss << "  \"name\": \"" << scenario.name << "\",\n";
    ss << "  \"width\": " << scenario.width << ",\n";
    ss << "  \"height\": " << scenario.height << ",\n";
    ss << "  \"rvc\": { \"x\": " << scenario.rvcStart.x << ", \"y\": " << scenario.rvcStart.y
       << ", \"heading\": \"" << directionToString(scenario.rvcHeading) << "\" },\n";
    ss << "  \"obstacles\": [";
    for (std::size_t i = 0; i < scenario.obstacles.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << "{\"x\":" << scenario.obstacles[i].x << ",\"y\":" << scenario.obstacles[i].y << "}";
    }
    ss << "],\n";
    ss << "  \"dust\": [";
    for (std::size_t i = 0; i < scenario.dust.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << "{\"x\":" << scenario.dust[i].x << ",\"y\":" << scenario.dust[i].y << "}";
    }
    ss << "],\n";
    ss << "  \"ticks\": " << scenario.ticks << ",\n";
    ss << "  \"trigger\": \"" << scenario.trigger << "\",\n";
    ss << "  \"expectedPath\": [";
    for (std::size_t i = 0; i < scenario.expectedPath.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << "{\"x\":" << scenario.expectedPath[i].x << ",\"y\":" << scenario.expectedPath[i].y
           << "}";
    }
    ss << "]\n";
    ss << "}\n";
    return ss.str();
}

bool saveScenarioToFile(const std::string& path, const MapScenario& scenario) {
    try {
        writeFile(path, scenarioToJson(scenario));
        return true;
    } catch (...) {
        return false;
    }
}

void applyScenarioToMap(const MapScenario& scenario, MapModel& map) {
    map.setSize(scenario.width, scenario.height);
    map.setObstacles(scenario.obstacles);
    map.setDust(scenario.dust);
    map.setRvcStart(scenario.rvcStart, scenario.rvcHeading);
}

}  // namespace rvc
