## Description
CS512 Funtime is a ground-breaking addition to the world of video games. Move your little character to collect coins through 10 levels of fun, and avoid sleepy zombies who will ruin your day! If the game is too hard, simply open the level editor and retool the level layouts to your liking. The world is your oyster!

### Controls:
    - Use arrow keys and Enter to navigate menus
    - Use WASD to control the hero
    - Use the mouse and other listed controls to work the level editor

---

## Requirements

### Make sure you have the following installed:
- Install GCC compiler to compile source code: [Follow instructions here](https://code.visualstudio.com/docs/cpp/config-mingw)
- CMake is required to automatically download dependencies and build the project. [Download it here](https://cmake.org/)
- You can either download the sourcecode from the GitHub webpage or clone using [Git](https://git-scm.com/)
    - you would run in the terminal "git clone [link to the repository webpage]"
---

## Building and Running the Program

To build the program, run the following commands in your terminal while in the source code's directory:
```bash
cmake -B build
cmake --build build
```
The CMake script will automatically download the raylib dependancies and build the executable.

After it finishes building, run the exe file found in the build/debug folder. 

## Issues

- There's room for more levels but they were not yet created
- The game requires the asset directory to be located 2 directories up from its location
- Menus used to support the mouse but it got broken
- If a level has no coins it goes to the next level immediately (weird)