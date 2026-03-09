

## Project Structure

```
Game/
├── src/               C source files
│   ├── main.c         Entry point
│   ├── game.c         Main game loop and command dispatcher
│   ├── player.c       Player stats, inventory, flags
│   ├── world.c        Location graph and movement
│   ├── dialogue.c     Dialogue tree and interactive conversations
│   ├── story.c        Chapter progression and endings
│   ├── ui.c           Terminal rendering and menus
│   └── utils.c        String, file, and random helpers
├── include/           Header files
├── assets/
│   ├── story.txt      Narrative content
│   └── locations.txt  Location definitions
├── docs/
│   └── DESIGN.md      Full game design document
└── CMakeLists.txt     Cross-platform build configuration
```

## Building

### Prerequisites

- CMake ≥ 3.16
- A C11-capable compiler (GCC, Clang, or MSVC)

### Steps

```bash
# Clone the repository
git clone https://github.com/RussellChr/horrorgame.git
cd horrorgame

# Configure and build (Release mode)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run the game
./build/paper_lily
```

### Debug build with AddressSanitizer

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build build
./build/paper_lily
```

## Running

```
Usage: 
  -n <name>    Player character name (default: "Stranger")
  -a <path>    Path to assets directory (default: assets)
  -s           Enable slow (typewriter) text output
  -c           Disable ANSI colour output
  -h           Show this help
```

Example:

```bash
```

## Gameplay

Type commands at the `>` prompt:

| Command             | Action                        |
|---------------------|-------------------------------|
| `north` / `n`       | Move north                    |
| `south` / `s`       | Move south                    |
| `east` / `e`        | Move east                     |
| `west` / `w`        | Move west                     |
| `up` / `down`       | Move vertically               |
| `look` / `l`        | Describe current room         |
| `take <item>`       | Pick up an item               |
| `use  <item>`       | Use an item                   |
| `talk` / `t`        | Talk to a nearby character    |
| `inventory` / `i`   | Show your items               |
| `status` / `stats`  | Show your character stats     |
| `help` / `h` / `?`  | List all commands             |
| `quit` / `q`        | Quit the game                 |

## Endings

There are four possible endings depending on your choices, stats, and story
flags:

- **Escape** – Get out with Lily before it's too late
- **Sacrifice** – Give yourself so Lily can be free
- **Truth** – Uncover the full secret and break the curse
- **Corruption** – Let the darkness win

Full details are in [`docs/DESIGN.md`](docs/DESIGN.md).

## License

This project is provided for educational and entertainment purposes.
