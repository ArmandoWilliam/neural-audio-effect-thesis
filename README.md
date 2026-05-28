This is my thesis project for Master in Sound Engineering at Tor Vergata in Rome. It's a custom made neural network based guitar distortion effect using the JUCE framework.

# Project Setup

## Prerequisites
- macOS with Xcode Command Line Tools (`xcode-select --install`)
- CMake 3.22+ (`brew install cmake`)
- Git
- VS Code with C/C++ and CMake Tools extensions (Microsoft)

## Clone JUCE
```bash
mkdir -p ~/GIT
cd ~/GIT
git clone https://github.com/juce-framework/JUCE.git
```
Important: the JUCE path must have no spaces, no accents, and must not be in an iCloud-synced folder.

## Clone the project
```bash
cd ~/GIT
git clone https://github.com/ArmandoWilliam/neural-audio-effect-thesis.git
cd neural-audio-effect-thesis
```

## CMake configuration
The `CMakeLists.txt` references JUCE via absolute path:
```cmake
add_subdirectory(/Users/armando/GIT/JUCE ${CMAKE_BINARY_DIR}/JUCE)
```
If your JUCE is in a different location, update this line accordingly.

## First build
```bash
cd ~/GIT/neural-audio-effect-thesis
cmake -B build -G "Unix Makefiles"
cmake --build build -j 4
```
The first build takes 10-15 minutes (compiles all JUCE modules). Subsequent builds are incremental and take seconds.

## Verify the plugin was built
```bash
ls build/NeuralGuitar_artefacts/Standalone/
ls ~/Library/Audio/Plug-Ins/Components/ | grep NeuralGuitar
ls ~/Library/Audio/Plug-Ins/VST3/ | grep NeuralGuitar
```

## VS Code IntelliSense setup
Add to `CMakeLists.txt` after `project(...)`:
```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```
Create `.vscode/c_cpp_properties.json`:
```json
{
    "configurations": [
        {
            "name": "Mac",
            "compileCommands": "${workspaceFolder}/build/compile_commands.json",
            "cStandard": "c17",
            "cppStandard": "c++20",
            "intelliSenseMode": "macos-clang-x64"
        }
    ],
    "version": 4
}
```
Rebuild with `cmake -B build -G "Unix Makefiles"` then reload VS Code.

---

# How to Build

## After modifying CMakeLists.txt
```bash
cd ~/GIT/neural-audio-effect-thesis
cmake -B build -G "Unix Makefiles"
cmake --build build -j 6
```

## After modifying .cpp or .h files only
```bash
cd ~/GIT/neural-audio-effect-thesis
cmake --build build -j 6
```

## Clean build (from scratch, using 6 parallel cores)
```bash
cd ~/GIT/neural-audio-effect-thesis
rm -rf build
cmake -B build -G "Unix Makefiles"
cmake --build build -j 6
```

## Running the Standalone for testing
```bash
./build/NeuralGuitar_artefacts/Standalone/NeuralGuitar.app/Contents/MacOS/NeuralGuitar
```
Launch from terminal to see DBG() output in the console.

## Filtering build errors only
```bash
cmake --build build -j 6 2>&1 | grep "error:"
```

## Debug build (development and testing, DBG() enabled)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
cmake --build build -j 4
```

## Release build (performance testing and final delivery)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cmake --build build -j 4
```

## Release build (performance testing and final delivery)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cmake --build build -j 6
```
