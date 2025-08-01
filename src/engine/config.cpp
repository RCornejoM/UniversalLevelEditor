#include "config.h"
#include "globals.h"
#include <fstream>
#include <iostream>
#include <sstream>

json gCurrentConfig;
std::string gCurrentConfigPath;
EditorConfig config;

bool configLoadFailed = false;
std::string configErrorMessage;

void LoadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        configLoadFailed = true;
        configErrorMessage = "[Config] Failed to open: " + filename;
        return;
    }

    try {
        file >> gCurrentConfig;

        if (gCurrentConfig.is_null()) {
            configLoadFailed = true;
            configErrorMessage = "[Config] JSON is null!";
            return;
        }

        if (!gCurrentConfig.is_object()) {
            configLoadFailed = true;
            configErrorMessage = "[Config] Loaded JSON is not an object!";
            return;
        }

        // Only now parse into config struct safely
        config = gCurrentConfig.get<EditorConfig>();
    } catch (const std::exception& e) {
        configLoadFailed = true;
        configErrorMessage = std::string("[Config] JSON parse or conversion error: ") + e.what();
    }
}

std::vector<GameProfile> gGameProfiles = {
};

int gSelectedGameIndex = 0;
