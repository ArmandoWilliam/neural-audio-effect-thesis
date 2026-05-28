This is my thesis project for Master in Sound Engineering at Tor Vergata in Rome. It's a custom made neural network based guitar distortion effect using the JUCE framework.

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
cmake --build build -j 6
```

## Release build (performance testing and final delivery)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
cmake --build build -j 6
```
