# Arcade Game Engine
 
A custom 2D game engine written in C++17 on top of SDL2, with four classic arcade games built on it: Breakout, Tetris, Asteroids and Pacman.
 
## Overview
 
The engine does its own rendering. Shapes, sprites, text and animations are drawn by the CPU into a pixel buffer, and SDL2 is only used for the window, input, timing and getting that buffer onto the screen. On top of that sit a scene stack, an input layer, a text based asset format and the game code. An arcade style menu lets you pick which game to play.
 
The game window is 224 by 288 logical pixels, scaled up 3 times to 672 by 864.
 
## Games
 
### Breakout
* Three levels loaded from a text file
* Blocks that take one, two or three hits, plus blocks that cannot be destroyed
* Three lives, game over and restart
* Ball collisions use edge normals and reflection, and the paddle has special handling for its corners
* Scores saved to a high score file
### Tetris
* All seven tetrominoes (I, O, T, L, J, S, Z) with rotation
* Line clearing, 100 points per line
* Next piece preview and a ghost piece that shows where the current piece will land
* A snap key that drops the piece straight onto the ghost
* Game over and restart, with scores saved to a high score file
### Asteroids
* A ship that rotates and thrusts, keeps drifting after thrust and slows down gradually
* Up to three missiles on screen at once
* Asteroids spawn off screen in three sizes and drift toward the middle of the screen
* Large and medium asteroids split into two smaller ones when shot. The pieces do not respawn, while the main asteroids respawn after they are destroyed or leave the screen
* Points depend on size (100 for small, 50 for medium, 10 for large), lives are shown as ship icons, and the ship plays an explosion animation when it is hit
* Collisions use axis aligned bounding boxes
* Scores saved to a high score file
### Pacman
* A tile based level loaded from a text file and drawn over a background image
* Pellets, power pellets that make the ghosts vulnerable, and bonus items
* Four ghosts (Blinky, Pinky, Inky and Clyde), each with its own scatter corner and chase behaviour, driven by a state machine: waiting in the pen, leaving the pen, scatter, chase and returning to the pen
* Ghosts are released from the pen on a timer
* Three lives, level progression, a ready screen and game over text
## Engine Features and Design
 
* **Fixed timestep loop.** The game updates every 10 ms from a time accumulator, and the frame time is clamped to 300 ms so a long pause cannot trigger a burst of catch up updates.
* **Scene stack.** The app keeps a stack of scenes. A `GameScene` wraps any `Game`, menus are scenes too, and pushing or popping a scene switches input to that scene's key bindings.
* **Software rendering.** A pixel back buffer with lines, circles, rectangles, triangles and stars (outlined or filled), sprites and bitmap font text. The buffer is uploaded to an SDL texture each frame and presented with vsync.
* **Sprites and animation.** BMP sprite sheets described by text files, animations defined in text files, and sprites drawn with rotation and a colour tint.
* **Input.** Each scene owns a `GameController` that maps keys to lambdas. Mouse motion and button events are supported by the engine as well.
* **Text based assets.** A small command parser (`FileCommandLoader`) reads files made of commands such as `:level`, `:block` and `:tile`. Breakout levels, the Pacman level, sprite sheets and animations all use it.
* **Collision.** Bounding box overlap tests, plus edge based collision with normals and reflection for Breakout.
* **High scores.** Scores are written to plain text files, one per game (Breakout, Tetris and Asteroids).
* **Ownership.** Scenes are held by `std::unique_ptr`, and pointers that do not own their target are named with a `noptr` prefix in the code, for example `mnoptrWindow`.
## Project Structure
 
```
ArcadeApp/
    ArcadeApp.sln
    SDL2.dll
    ArcadeApp/
        App/          Application singleton, main loop, menu button
        Assets/       Sprite sheets, animations, font and level files
        Games/
            Asteroids/
            Breakout/
            Pacman/
            Tetris/
        Graphics/     Screen, pixel buffer, BMP loading, sprites, animation, bitmap font
        Input/        Key and mouse handling
        Scenes/       Scene classes and menus
        Shapes/       Line, circle, rectangle, triangle and star
        Utils/        Vec2D, file parser, high score file, helpers
```
 
## Building and Running
 
### Requirements
* Windows 10 or 11
* Visual Studio 2022 (v143 toolset)
* SDL2 development libraries, version 2.32.8 (the version the project was set up with)
* x64 only, because the project links the x64 SDL libraries
### Steps
 
1. Download the SDL2 Visual C++ development package from [libsdl.org](https://www.libsdl.org/) and extract it somewhere.
2. Open `ArcadeApp/ArcadeApp.sln` in Visual Studio.
3. The project file contains absolute paths from my machine, both for SDL2 and for the project's own folders (Games, Graphics, Scenes and so on). Open Project Properties and update **C/C++, Additional Include Directories** and **Linker, Additional Library Directories** to match where SDL2 and the repo live on your machine.
4. Choose Debug or Release, select x64, and build.
5. Run the game. It loads its assets from an `Assets` folder next to the executable and needs `SDL2.dll` in the same folder. A copy of `SDL2.dll` is included in the repo.
## Controls
 
* **Menus:** Up and Down arrows to move, A to select
* **Breakout:** Left and Right arrows move the paddle. A serves the ball and restarts after game over. S goes back to the menu while serving or at game over
* **Tetris:** Left and Right arrows move the piece. A rotates it, and restarts after game over. E snaps the piece down onto the ghost. S goes back to the menu
* **Asteroids:** Left and Right arrows rotate, Up thrusts, A fires and restarts after game over.
* **Pacman:** Arrow keys steer. S goes back to the menu at game over
* Close the window to quit
## Data Files
 
Breakout levels are defined in `Assets/BreakoutLevels.txt`. Each level declares its block types and then lays them out as a grid:
 
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
xxxxxxxxxxxxx-
...
```
 
A block with an `hp` of -1 can never be destroyed, because the code only reduces hit points while they are above zero.
 
The Pacman level in `Assets/Pacman_level.txt` uses the same style of file. It defines tile types (`:tile`, `:tile_symbol`, `:tile_collision`, `:tile_width`) and a background bitmap (`:bg_image`).
 
## Extending the Engine
 
To add a new game:
 
1. Create a class that inherits from `Game`
2. Implement `Init(GameController&)`, `Update(uint32_t dt)`, `Draw(Screen&)` and `GetName()`
3. Bind input actions in `Init()`
4. Add a menu entry and a matching case in `ArcadeScene`, which wraps your game in a `GameScene`
## Known Limitations
 
* Windows and Visual Studio only, and the project file uses absolute paths from my machine
* Rendering is done on the CPU at a fixed resolution
* No audio
* No gamepad support
## Credits and License
 
This is an educational project. SDL2 is licensed under the zlib license.