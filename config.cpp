#include "config.h"
#include "globals.h"
#include <fstream>
#include <iostream>
#include <sstream>

json gCurrentConfig;
std::string gCurrentConfigPath;
EditorConfig config;

void LoadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Config] Failed to open: " << filename << std::endl;
        return;
    }

    try {
        file >> gCurrentConfig;
        gCurrentConfigPath = filename;
    } catch (const std::exception& e) {
        std::cerr << "[Config] JSON parse error: " << e.what() << std::endl;
    }
    config = gCurrentConfig.get<EditorConfig>();
}

std::vector<GameProfile> gGameProfiles = {
    {"Template Game", "./Games/Template Game/template-game.config"},
    {"Donkey Kong", "./Games/Donkey Kong Country/donkey-kong-country.config"},
    {"Super Mario World", "./Games/Super Mario Woeld/super-mario-world.config"},
    {"Metal Warriors", "./Games/Metal Warriors/metal-warriors.config"},
};

int gSelectedGameIndex = 0;
