A C++ game built with raylib for CS512.  
In this game, you traverse through a series of levels, collecting coins to earn points while avoiding enemies found in the map.

---

## Features:
- Move character around using WASD.
- Earn points by collecting coins found around the map.
- Moving near enemies will cause them to start moving. Avoid them!
- Select between multiple different premade levels.

---

## Requirements

Make sure you have the following installed:

##Dependencies
- C++17 or newer
- raylib library

## Installing Raylib (Linux / Ubuntu)
```bash
sudo apt install libraylib-dev
```

---

## Building and Running the Program

To build the program, run the following commands in your terminal while in this directory:
```bash
cmake -B build
cmake --build build
```

After it finishes building, run the exe file found in the build/debug folder. 