# Gamo

Gamo is a cross-platform game engine written in **C (C17)** and built with **CMake**.  
It follows a clean **engine / app architecture**, uses **GLFW** for windowing, and is designed to explore low-level engine patterns without relying on C++ abstractions.

---

## Features

- C17
- Cross-platform architecture (Linux supported, Windows planned)
- CMake build system
- Engine and App separation
- GLFW backend
- OpenGL context support
- Vendor dependencies via git submodules
- clang-format and clang-tidy integration
- Platform abstraction layer

---

## Project Structure

Gamo/
├── engine/        # Engine source and public headers
│   ├── include/
│   └── src/
├── app/           # Application using the engine
│   └── src/
├── vendor/        # Third‑party libraries (GLFW, etc.)
├── build/         # Generated build files
├── CMakeLists.txt
├── LICENSE
└── README.md

---

## Requirements (Linux)

Install compiler, CMake, GLFW, OpenGL, and clang tools.

### Fedora

sudo dnf install \
    gcc \
    cmake \
    make \
    git \
    glfw-devel \
    mesa-libGL-devel \
    clang \
    clang-tools-extra

### Ubuntu / Debian

sudo apt install \
    build-essential \
    cmake \
    git \
    libglfw3-dev \
    libgl1-mesa-dev \
    clang \
    clang-tidy \
    clang-format

### Arch Linux / Manjaro

sudo pacman -S \
    base-devel \
    cmake \
    git \
    glfw \
    mesa \
    clang

### openSUSE

sudo zypper install \
    gcc \
    cmake \
    make \
    git \
    glfw-devel \
    Mesa-libGL-devel \
    clang \
    clang-tools

### Alpine

sudo apk add \
    build-base \
    cmake \
    git \
    glfw-dev \
    mesa-dev \
    clang \
    clang-extra-tools

---

## Clone

git clone --recursive <repository-url>
cd gamo

If already cloned:

git submodule update --init --recursive

---

## Build

cmake -B build
cmake --build build

---

## Run

./build/app/Gamo

---

## Development

Generate compile database:

cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

Format code:

clang-format -i $(find . -name "*.c" -o -name "*.h")

Lint code:

clang-tidy -p build

---

## For windows build (Not tested)

```
sudo dnf install mingw64-gcc mingw64-cmake wine
```

```
cmake -B build-win \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw64.cmake
```

```
cmake --build build-win
```

```
wine build-win/app/Gamo.exe
```

## Architecture

The engine exposes clean interfaces while hiding backend implementations.

App
↓
Engine API
↓
Platform abstraction
↓
GLFW backend
↓
Operating system

Opaque handles are used to hide implementation details.

---

## License

MIT License. See LICENSE file.
