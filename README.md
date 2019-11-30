[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Voxel Editor

Small voxel map editor with an Entity Component System (ECS) architecture.

Supports WASM build so it can be played on any web-browser like Chrome, Firefox or Safari. Just run the content of the `www` folder on a web server.

## Table of Contents

+ [Getting started](#Getting-Started)

## Getting Started

### Prerequisites

You need to install [Cmake](https://cmake.org/) to build the project, and a C++ compiler which handles C++17

### Build on desktop

You can handle the `CMakeLists.txt` in any way you like, it will download the dependecies by itself. 

Here's some way to use it :

#### `Option 1: CLI`

```bash
cmake .
make
```

#### `Option 2: Visual Studio (Windows only)`

Open this folder with the `CMake...` option in file->open on Visual Studio, and run the project.

#### `Option 3: VSCode`

Use the `CMakeTools` plugin, build with `f7` then run with `f5` (But be carefull to be on the right platform, there is a launch file for windows and for linux).

### Build for the Web as WASM (linux only)

This project support Web Assembly, so it can run in a browser like Google Chrome or Firefox !

The build steps are the same for any platform (however it seems broken for now on Windows). Do not forget to delete `CMakeCache.txt` if there is one on the folder.

#### Conan

You need to install [Conan](https://conan.io/). It will download and run the Emscripten compiler.

Then, add bincrafters repo for conan to know where to get Emscripten.

```bash
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

#### Build

```bash
conan install ./wasm.recipe.py --build missing --install-folder wasm -pr ./wasm.profile
conan build ./wasm.recipe.py --build-folder wasm
```

#### Run

You can then copy the files inside `wasm/bin` into `www` and run this folder with a simple web-server. On this exemple, we are creating a simple one with python :

```bash
cp -a wasm/bin/. www
cd www
python3 -m http.server -b 127.0.0.1
```

Then open [your local server](http://127.0.0.1:8000/) to see the project.
