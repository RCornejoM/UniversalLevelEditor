#pragma once

#include <iostream>
#include "config.h"
#include "stamp.h"
#include <string>

void Initialize();
void Display();
void RenderTilesetWindow();
void RenderTileset();
void RenderTilemapWindow(std::string name);
void RenderTilemap();
void RenderStampPreview(const Stamp& stamp);
void RenderTilePreview();
void DrawConfigEditor();
json LoadJsonFile(const std::string& path);
