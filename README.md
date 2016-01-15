# MazeCraft
MazeCraft is a simple First Person game with maze-like elements, in a custom multi-platform game engine utilizing OpenGL for rendering. It is written in C++, and is designed with performance and scalability in-mind. This project was initially developed as part of an assignment for the Graphics course at University of Ioannina, and builds upon the RoPaSci II game engine.

## Supported Platforms
- Windows
- Linux (*64-bit only*)

### Platform & Hardware Notes
MazeCraft has only been tested on Windows 10 (*64-bit*) and on Ubuntu 14.04 Desktop (*64-bit*) with an AMD R9 290 GPU and an Intel i7 5930K CPU. Any other platform configurations are currently not guaranteed to work. 

## Building
Below you will find instructions on how to build MazeCraft on all supported platforms.

### Windows
#### Prerequisites
- Visual Studio 2013 (msvc120)
- Windows SDK

To build MazeCraft on Windows, simply clone (or download) the repository to some local folder, open the provided solution (*MazeCraft.sln*) with Visual Studio, select the configuration and platform of your choice (*Debug or Release, Win32 or x64*), and build the solution.

### Linux
#### Prerequisites
- CMake 3.3 (or newer)
- GCC 4.8 (or newer) 

To build MazeCraft on Linux follow the following steps:
```
$ git clone git@github.com:OrfeasZ/MazeCraft.git
$ cd MazeCraft
$ mkdir _build
$ cd _build
$ cmake ..
$ make
```

## Usage
Before using MazeCraft you need to make sure that all the required game data and libraries reside next to the built executable. On windows this process is automated, however on Linux you will have to manually create a `data` folder next to the built executable and copy all the contents of the project `Data` folder in it. Additionally, you will have to copy all files present in the `Depends/bin/elf64` directory next to the built executable.

After all required files are in place, simply launch MazeCraft by double-clicking on `MazeCraft.exe` on Windows, or by running `run.sh` on Linux.

## Authors
- Orfeas - Ioannis Zafeiris (2250)

## References
- "[Using Tasking to Scale Game Engine Systems](https://software.intel.com/en-us/articles/using-tasking-to-scale-game-engine-systems)" by Yannis Minadakis (Intel)

## Special Thanks
- Joana Castanheiro for making the Warp Portal and Destroyed Block models