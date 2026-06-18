#include "rvc/MapJson.hpp"
#include "rvc/SimulationEngine.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static bool readManifest(const std::string& path, std::vector<std::string>& out) {
    std::ifstream in(path);
    if (!in) {
        return false;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        out.push_back(line);
    }
    return !out.empty();
}

int main(int argc, char* argv[]) {
    std::string manifestPath = "System-Test/scenarios/manifest.txt";
    if (argc > 1) {
        manifestPath = argv[1];
    }

    std::vector<std::string> files;
    if (!readManifest(manifestPath, files)) {
        std::cerr << "Manifest not found or empty: " << manifestPath << "\n";
        return 1;
    }

    int passed = 0;
    int failed = 0;
    std::vector<std::string> failures;

    for (const auto& file : files) {
        rvc::MapScenario scenario;
        if (!rvc::loadScenarioFromFile(file, scenario)) {
            ++failed;
            failures.push_back(file + " (load error)");
            continue;
        }

        rvc::SimulationEngine engine;
        engine.loadScenario(scenario);
        engine.runTicks(scenario.ticks > 0 ? scenario.ticks : 1);

        if (!engine.pathMatchesExpected()) {
            ++failed;
            failures.push_back(scenario.id + " path mismatch");
            continue;
        }
        ++passed;
        std::cout << "[PASS] " << scenario.id << " — " << scenario.name << "\n";
    }

    std::cout << "\nSystem test summary: " << passed << " passed, " << failed << " failed\n";
    for (const auto& f : failures) {
        std::cout << "  FAIL: " << f << "\n";
    }
    return failed == 0 ? 0 : 1;
}
