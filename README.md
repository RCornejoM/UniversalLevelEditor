# UniversalLevelEditor

A minimal OpenGL + ImGui-based level editor for tile-based games, built for editing SNES games.

## Features

- Loads tilesets from image files.
- Loads tilemaps from game ROMs.
- Supports horizontal and vertical flipping of tiles.
- Customizable tile size.
- Configurable direction of tilemap load (top-to-bottom or left-to-right).
- Copy & paste.
- New games can be added from the editor.

## Requirements

- C++ compiler (supporting C++11 or higher)
- OpenGL, GLUT, GLEW
- [Dear ImGui](https://github.com/ocornut/imgui)

## Build Instructions

```bash
make        # Builds the default image viewer
make new    # Builds the tilemap editor
make clean  # Cleans build artifacts
```

## Config Format

Here’s an example `config.json`:

```json
{
    "map_filename": "game.sfc",
    "tileset_filename": "Tilesets/tileset.png",
    "tile_size": 64,
    "tileset_total": 640,
    "map_rows": 32,
    "map_cols": 64,
    "screen_width_tiles": 16,
    "screen_height_tiles": 16,
    "map_address": "0x12345",
    "top_to_bottom": true
}
```

## Controls

- Select a tile in the tileset window.
- Click on the tilemap to place the selected tile.
- Use ImGui checkboxes to flip tiles horizontally or vertically.

## License

No license

---

Contributions welcome!

## ⚠️ Legal Notice
This tool is intended for use **only with legally obtained game copies**.
Please **dump your own cartridges** or ROMs using legal methods.
For SNES games, you must use legally dumped `.sfc` files from cartridges you own.
