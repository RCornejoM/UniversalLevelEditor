# Universal Level Editor

A minimal OpenGL + ImGui-based tilemap editor built for editing SNES games.

## Features

- Load tilesets from image files
- Load tilemaps from `.sfc` ROMs
- Configurable tilemap load direction
- Horizontal/vertical flipping
- Stamp copy & paste

## Requirements

- C++17 compiler
- CMake ≥ 3.10
- OpenGL
- GLEW
- freeglut
- pkg-config (Linux)

### Ubuntu/Debian

```bash
sudo apt install build-essential cmake libglew-dev freeglut3-dev libgl1-mesa-dev pkg-config
```

### Windows (MSYS2)

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-glew mingw-w64-x86_64-freeglut
```

## Build Instructions

```bash
git clone https://github.com/RCornejoM/UniversalLevelEditor.git
cd UniversalLevelEditor
mkdir build && cd build
cmake ..
cmake --build .
```

## Run

```bash
./UniversalLevelEditor
```


## Config Format

Example: assets/configs/donkey-kong-country.json

```json
{
  "map_filename": "./roms/donkey-kong-country.sfc",
  "map_rows": 16,
  "map_cols": 1344,
  "tile_size": 32,
  "tilemap_address": 1638400,
  "tileset_filename": "./assets/tilesets/donkey-kong-country.png",
  "top_to_bottom": true,
  "tileset_total": 640
}
```

## License

No license.

---

## ⚠️ Legal Notice
This tool is intended for use **only with legally obtained game copies**.
Please **dump your own cartridges** or ROMs using legal methods.
For SNES games, you must use legally dumped `.sfc` files from cartridges you own.
