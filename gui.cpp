#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"

#include "stb_image.h"
#include <fstream>
#include <iostream>

#include "globals.h"
#include "gui.h"
#include "stamp.h"
#include "tilemap.h"
#include "tileset.h"
#include "shaders.h"

#include "stamp.h"

#include "config.h"


struct UVRect {
    ImVec2 min;
    ImVec2 max;
};

UVRect GetTileUV(int tileIndex, bool hflip, bool vflip) {
    if (tileIndex >= tileset->getTotalTiles()) return UVRect{{0, 0}, {0, 0}};

    int tilesPerRow = tileset->getWidthTiles();  // e.g. 8
    int tileSize = tileset->getTileSize();       // e.g. 32 px

    int x = tileIndex % tilesPerRow;
    int y = tileIndex / tilesPerRow;

    // Convert tile coords to pixel coords
    float u0 = (x * tileSize) / (float)tileset->getWidth();   // image width in pixels
    float v0 = (y * tileSize) / (float)tileset->getHeight();
    float u1 = ((x + 1) * tileSize) / (float)tileset->getWidth();
    float v1 = ((y + 1) * tileSize) / (float)tileset->getHeight();

    if (hflip) std::swap(u0, u1);
    if (vflip) std::swap(v0, v1);

    return UVRect{{u0, v0}, {u1, v1}};
}

void Display() {
    glClearColor(0.15f, 0.21f, 0.22f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(gShaderProgram);
    glBindVertexArray(gVAO);
    glBindTexture(GL_TEXTURE_2D, tileset->getTextureID());

    // Setup ImGui frame
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();

    RenderTilemapWindow("Tilemap");
    RenderTilesetWindow();
    DrawConfigEditor();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glutSwapBuffers();
}

void RenderTilemapWindow(std::string name) {
    if (tilemap->hasLoaded()) {
    ImGui::Begin(name.c_str());

    ImGui::Text("Left click: place tile. Right click to copy.");
    ImGui::Text("Right-drag to copy an area.");

    // Tilemap related buttonssss
    if (ImGui::Button("Load Tilemap")) {
        tilemap->load(config.mapFilename,config.tilemapAddress,config.mapRows,config.mapCols,config.topToBottom);
    }
    if (ImGui::Button("Clear Map")) {
        // Clear tilemap vector properly
        tilemap->clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save Map")) {
        tilemap->save();
    }

    // Scrollable tilemap region
    ImVec2 childSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("Tilemap", childSize, true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    RenderTilemap();

    // --- Pan logic ---
    ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
    bool isPanning = ImGui::IsWindowHovered() && ImGui::IsMouseDown(2);

    if (isPanning) {
        float scrollX = ImGui::GetScrollX();
        float scrollY = ImGui::GetScrollY();
        ImGui::SetScrollX(scrollX - mouseDelta.x);
        ImGui::SetScrollY(scrollY - mouseDelta.y);
    }

    RenderStampPreview(stamp);
    ImGui::EndChild();

    ImGui::End();
    }
}

void RenderTilemap(){
    // Save old style
    ImVec2 prevItemSpacing2 = ImGui::GetStyle().ItemSpacing;
    ImVec2 prevFramePadding2 = ImGui::GetStyle().FramePadding;
    ImGui::GetStyle().ItemSpacing = ImVec2(0, 0);
    ImGui::GetStyle().FramePadding = ImVec2(0, 0);

    int rows = tilemap->rows();
    int cols = tilemap->cols();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Tile tile = tilemap->get(row,col);

            UVRect uv = GetTileUV(tile.index(), tile.hflip(), tile.vflip());

            ImGui::PushID(row * cols + col);
            char btn_id[32];
            snprintf(btn_id, sizeof(btn_id), "tmap_%d", row * cols + col);

            ImGui::ImageButton(
               btn_id,
               (ImTextureID)(uintptr_t)tileset->getTextureID(),
               ImVec2(config.tileSize, config.tileSize),
               uv.min,
               uv.max
            );

            // Always allow right drag to copy stamp
            if (ImGui::IsItemHovered()) {
                if (ImGui::IsMouseClicked(1)) {  // Right-click starts selection
                    isSelecting = true;
                    selectStartRow = row;
                    selectStartCol = col;
                    selectEndRow = row;
                    selectEndCol = col;
                } else if (isSelecting && ImGui::IsMouseDown(1)) {
                    selectEndRow = row;
                    selectEndCol = col;
                } else if (isSelecting && ImGui::IsMouseReleased(1)) {
                    isSelecting = false;
                    stamp.copyFromTilemap(selectStartRow, selectStartCol, selectEndRow, selectEndCol);
                }
            }

            ImGui::PopID();

            if (col < cols - 1)
                ImGui::SameLine();

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                stamp.pasteToTilemap(row, col);
            }
        }
    }

    ImGui::GetStyle().ItemSpacing = prevItemSpacing2;
    ImGui::GetStyle().FramePadding = prevFramePadding2;
}

void RenderTilesetWindow() {
    if (tilemap->hasLoaded()) {
    ImGui::Begin("Tileset");
    ImGui::Text("Selected Tile: 0x%03X (%d)", gSelectedTile & 0x03FF, gSelectedTile & 0x03FF);

    if (ImGui::Button("Flip Horizontally"))
        stamp.flipHorizontally();

    if (ImGui::Button("Flip Vertically"))
        stamp.flipVertically();

    ImGui::Separator();

    ImGui::Text("Pick a tile:");
    RenderTileset();

    ImGui::End();
    }
}

void RenderTileset(){
    ImVec2 childSize = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("TileList", childSize, true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    constexpr int TILES_PER_ROW = 8;

    for (int i = 0; i < config.tilesetTotal; ++i) {
        ImGui::PushID(i);
        char btn_id[32];
        snprintf(btn_id, sizeof(btn_id), "tile_%d", i);

        UVRect uv = GetTileUV(i, false, false);  // no flip when browsing

        bool clicked = ImGui::ImageButton(
           btn_id,
           (ImTextureID)(uintptr_t)tileset->getTextureID(),
           ImVec2(config.tileSize, config.tileSize),
           uv.min,
           uv.max
        );

        if (clicked){
            uint16_t tileData = i;
            stamp.setSingleTile(tileData);
        }

        ImGui::PopID();

        if ((i % TILES_PER_ROW) < TILES_PER_ROW - 1)
            ImGui::SameLine();
    }
    ImGui::EndChild();
}

void RenderStampPreview(const Stamp& stamp) {
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 childPos = ImGui::GetCursorScreenPos();
    ImVec2 relativePos = ImVec2(mousePos.x - childPos.x, mousePos.y - childPos.y);

    int centerCol = (int)(relativePos.x / config.tileSize);
    int centerRow = (int)(relativePos.y / config.tileSize);

    auto [startRow, startCol] = stamp.getCenter(centerRow, centerCol);

    for (int r = 0; r < stamp.rows(); ++r) {
        for (int c = 0; c < stamp.cols(); ++c) {
            Tile tileData = stamp.get(r, c);

            UVRect uv = GetTileUV(tileData.index(), tileData.hflip(), tileData.vflip());

            ImVec2 posMin = ImVec2(
                childPos.x + (startCol + c) * config.tileSize,
                childPos.y + (startRow + r) * config.tileSize
            );
            ImVec2 posMax = ImVec2(posMin.x + config.tileSize, posMin.y + config.tileSize);

            ImGui::GetForegroundDrawList()->AddImage(
                (ImTextureID)(uintptr_t)tileset->getTextureID(),
                posMin, posMax,
                uv.min, uv.max,
                IM_COL32(255, 255, 255, 180)
            );
        }
    }
}


// Setup quad, load texture, init ImGui etc.
void Initialize() {
    glewInit();
    // --- Shader ---
    GLuint vs = CompileShader(GL_VERTEX_SHADER, VERTEX_SHADER_SRC);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
    gShaderProgram = glCreateProgram();
    glAttachShader(gShaderProgram, vs);
    glAttachShader(gShaderProgram, fs);
    glLinkProgram(gShaderProgram);
    glDeleteShader(vs); glDeleteShader(fs);

    // --- Fullscreen quad (2 triangles) ---
    float quad[] = {
        // x,    y,    u,   v
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };
    unsigned int idx[] = {0,1,2, 2,3,0};
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    glGenBuffers(1, &gVBO);
    GLuint EBO; glGenBuffers(1,&EBO);

    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    // --- Texture ---
    // Load image
    int n = 0;
    std::cout << "Loading tileset image: " << tileset->getFilename() << std::endl;
    unsigned char* data = stbi_load(tileset->getFilename().c_str(), &tileset->width, &tileset->height, &n, 4);
    if (!data) {
        std::cerr << "Tileset image load failed!\n";
        exit(1);
    }

    std::cout << "Image width: " << tileset->width << std::endl;
    std::cout << "Image height: " << tileset->height << std::endl;
    std::cout << "Image channels requested: 4" << std::endl;
    std::cout << "Image channels returned: " << n << std::endl;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &tileset->textureID);
    std::cout << "Generated texture ID: " << tileset->textureID << std::endl;

    glBindTexture(GL_TEXTURE_2D, tileset->textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tileset->width, tileset->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    std::cout << "Texture parameters set: MIN_FILTER=NEAREST, MAG_FILTER=NEAREST, WRAP_S=CLAMP_TO_EDGE, WRAP_T=CLAMP_TO_EDGE\n";

    stbi_image_free(data);
    std::cout << "Finished loading texture and freed image data.\n";
}

void RenderGameSelectorUI() {
    if (ImGui::Begin("Game Selector")) {
        std::vector<const char*> names;
        for (const auto& game : gGameProfiles)
            names.push_back(game.name.c_str());

        if (ImGui::Combo("Select Game", &gSelectedGameIndex, names.data(), names.size())) {
            LoadConfig(gGameProfiles[gSelectedGameIndex].configPath);
            tilemap->load(config.mapFilename,config.tilemapAddress, config.mapRows, config.mapCols,config.topToBottom);
        }
    }
    ImGui::End();
}
// Helper to edit and sync a std::string field with ImGui InputText
bool EditStringField(const std::string& label, std::string& field, json& jsonObj) {
    char buffer[256];
    std::strncpy(buffer, field.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    bool changed = ImGui::InputText(label.c_str(), buffer, sizeof(buffer));
    if (changed) {
        field = buffer;
        jsonObj[label] = buffer;
    }
    return changed;
}

void DrawConfigEditor() {
    ImGui::Begin("Game Config");

    static int selectedGameIndex = -1;
    // Game selection
    if (ImGui::BeginCombo("Select Game",
        (selectedGameIndex >= 0 && selectedGameIndex < (int)gGameProfiles.size())
            ? gGameProfiles[selectedGameIndex].name.c_str()
            : "Choose Game..."))
    {
        for (size_t i = 0; i < gGameProfiles.size(); ++i) {
            bool isSelected = (i == (size_t)selectedGameIndex);
            if (ImGui::Selectable(gGameProfiles[i].name.c_str(), isSelected)) {
                selectedGameIndex = (int)i;

                gCurrentConfigPath = gGameProfiles[i].configPath;
                config = LoadJsonFile(gCurrentConfigPath).get<EditorConfig>();

                // Immediately load tileset and tilemap when a game is selected
                tileset->load(config.tilesetFilename, config.tileSize);
                tilemap->load(config.mapFilename, config.tilemapAddress,
                              config.mapRows, config.mapCols, config.topToBottom);
            }

            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
    ImGui::Text("Config file: %s", gCurrentConfigPath.c_str());

    // Helper for editable string
    auto EditString = [](const char* label, std::string& str) {
        char buf[256];
        std::strncpy(buf, str.c_str(), sizeof(buf));
        buf[sizeof(buf)-1] = '\0';
        if (ImGui::InputText(label, buf, sizeof(buf))) {
            str = buf;
            return true;
        }
        return false;
    };

    if (EditString("Tileset Filename", config.tilesetFilename)) {

        tileset->load(config.tilesetFilename,config.tileSize);
    }

    if (EditString("Map Filename", config.mapFilename)) {
        tilemap->load(config.mapFilename,config.tilemapAddress,config.mapRows,config.mapCols,config.topToBottom);
    }

    // Integer and bool fields
    ImGui::InputInt("Tile Size", &config.tileSize);
    ImGui::InputInt("Tileset Total", &config.tilesetTotal);
    ImGui::InputInt("Map Rows", &config.mapRows);
    ImGui::InputInt("Map Cols", &config.mapCols);
    ImGui::Checkbox("Top to Bottom", &config.topToBottom);

    // Hex input for address
    static char addrBuf[32];
    snprintf(addrBuf, sizeof(addrBuf), "0x%X", config.tilemapAddress);
    if (ImGui::InputText("Tilemap Address", addrBuf, sizeof(addrBuf),
                         ImGuiInputTextFlags_CharsHexadecimal)) {
        config.tilemapAddress = std::strtoul(addrBuf, nullptr, 16);
    }

    if (ImGui::Button("Save Config")) {
        std::ofstream out(gCurrentConfigPath);
        if (!out) {
            std::cerr << "Failed to write config file: " << gCurrentConfigPath << "\n";
        } else {
            json j = config;
            out << j.dump(4);
        }
    }

    ImGui::End();
}

json LoadJsonFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open config file: " << path << "\n";
        return {};
    }
    json j;
    file >> j;
    return j;
}
