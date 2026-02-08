# ArcadeGameEngine

# Arcade Game Engine

A custom 2D game engine built in C++ with SDL2, designed for creating classic arcade-style games.

## Overview

This project is a lightweight game engine that provides core functionality for building 2D arcade games. It includes implementations of Breakout and Tetris as demonstration games, along with a flexible framework for adding additional games.

## Features

- **Custom 2D Graphics System**: Software rendering with pixel-level control
- **Shape Primitives**: Lines, circles, rectangles, triangles, and stars
- **Collision Detection**: AABB (Axis-Aligned Bounding Box) collision system
- **Input System**: Keyboard and mouse input handling
- **Scene Management**: Stack-based scene system for game states
- **Score Tracking**: High score file system for persistent data
- **Game Framework**: Abstract base classes for easy game development

## Implemented Games

### Breakout
- Classic brick-breaking gameplay
- Multiple levels loaded from configuration files
- Lives system and score tracking
- Paddle physics with corner bounce mechanics
- High score persistence

### Tetris
- Standard Tetrimino shapes (I, O, T, L, J, S, Z)
- Rotation system with boundary checking
- Automatic falling mechanism
- Movement controls (left, right, rotate)

## Project Structure

```
ArcadeApp/
├── App/              # Application core and main loop
├── Games/            # Game implementations
│   ├── Breakout/     # Breakout game components
│   └── Tetris/       # Tetris game components
├── Graphics/         # Rendering system
├── Input/            # Input handling
├── Scenes/           # Scene management
├── Shapes/           # Geometric primitives
└── Utils/            # Utility classes and helpers
```

## Building the Project

### Prerequisites
- Visual Studio 2022 (v143 toolset)
- SDL2 library
- Windows 10/11
- C++17 compiler

### Setup Instructions

1. **Install SDL**:
   - Download SDL2 from [libsdl.org](https://www.libsdl.org/)
   - Extract to `C:\Users\[YourUsername]\Documents\Libraries\SDL2-2.32.8\` or update paths in project settings

2. **Open the Project**:
   - Open `ArcadeApp.vcxproj` in Visual Studio

3. **Update Include/Library Paths** (if needed):
   - Right-click project → Properties
   - C/C++ → Additional Include Directories: Update SDL path
   - Linker → Additional Library Directories: Update SDL lib path

4. **Build**:
   - Select Debug or Release configuration
   - Choose x64 platform
   - Build Solution (Ctrl+Shift+B)

5. **Copy SDL DLL**:
   - Copy `SDL2.dll` or `SDL3.dll` to the output directory

## Controls

### Breakout
- **Left Arrow**: Move paddle left
- **Right Arrow**: Move paddle right
- **A Key**: Serve ball / Restart game (after game over)

### Tetris
- **Left Arrow**: Move piece left
- **Right Arrow**: Move piece right
- **A Key**: Rotate piece

## Configuration

### Breakout Levels
Levels are defined in `Assets/BreakoutLevels.txt`:
- Define block types with symbols, colors, and hit points
- Layout blocks using ASCII art
- Support for unbreakable blocks (hp: -1)

Example format:
```
:level
:block
:symbol x
:fillcolor 255 255 0 255
:hp 1
:width 14
:height 13
:layout 13
--xxxxxxxxx---
-xxxxxxxxxxx--
...
```

## Key Classes

### Core Systems
- **App**: Singleton application manager, main game loop
- **Screen**: Rendering interface with double buffering
- **ScreenBuffer**: Pixel buffer management
- **Scene**: Base class for game states

### Game Framework
- **Game**: Abstract interface for game implementations
- **GameScene**: Scene wrapper for games
- **GameController**: Input mapping and handling

### Shapes & Physics
- **Vec2D**: 2D vector math operations
- **AARectangle**: Axis-aligned rectangle collision
- **Circle**: Circular collision detection
- **Excluder**: Collision detection helper
- **BoundaryEdge**: Edge-based collision with normals

### Utilities
- **FileCommandLoader**: Parse custom file formats
- **HighScoreFile**: Score persistence system
- **Utils**: Math helpers (epsilon comparisons, time conversion)

## Architecture Highlights

- **Scene Stack**: Games are pushed/popped as scenes, enabling menus and state management
- **Component-Based Design**: Games compose objects like Ball, Paddle, Block
- **Event-Driven Input**: Actions bound to keys with lambda functions
- **File-Based Configuration**: Levels loaded from text files for easy editing

## Screen Resolution

- Default: 224x288 pixels (classic arcade resolution)
- Magnification: 3x (672x864 display window)
- Configurable in `ArcadeApp.cpp`

## Extending the Engine

To add a new game:

1. Create a new class inheriting from `Game`
2. Implement required methods:
   - `Init(GameController&)`
   - `Update(uint32_t dt)`
   - `Draw(Screen&)`
   - `GetName()`
3. Register input actions in `Init()`
4. Add game instantiation in `ArcadeScene`

## Known Limitations

- Software rendering (no hardware acceleration)
- Fixed-resolution rendering
- Limited to keyboard/mouse input
- Windows-only build configuration currently

## Future Enhancements

- Additional games (Asteroids, Pac-Man planned)
- Menu system for game selection
- Sound/audio support
- Cross-platform build support
- Hardware-accelerated rendering option

## License

This is an educational project. SDL2 is licensed under the zlib license.

## Credits

Built as a learning project for game engine architecture and classic arcade game mechanics.