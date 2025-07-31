# UniversalLevelEditor

A minimal OpenGL + ImGui-based level editor for tile-based games, built for editing SNES games.

## Features

- Loads tilesets from image files.
- Loads tilemaps from game ROMs.
- Configurable direction of tilemap load (top-to-bottom or left-to-right).
- Supports horizontal and vertical flipping of tiles.
- Copy & paste.

## Requirements

- C++ compiler (supporting C++11 or higher)
- OpenGL, GLUT, GLEW

## Build Instructions

```bash
make clean
make
```

## Config Format

Here’s an example `config.json`:

```json
{
{
    "map_filename": "./Roms/donkey-kong-country.sfc",
    "map_rows": 16,
    "map_cols": 1344,
    "tile_size": 32,
    "tilemap_address": 1638400,
    "tileset_filename": "./Games/Donkey Kong Country/donkey-kong-country-tileset.png",
    "top_to_bottom": true,
    "tileset_total": 640
}
```

## License

No license

---

Contributions welcome!

## ⚠️ Legal Notice
This tool is intended for use **only with legally obtained game copies**.
Please **dump your own cartridges** or ROMs using legal methods.
For SNES games, you must use legally dumped `.sfc` files from cartridges you own.
