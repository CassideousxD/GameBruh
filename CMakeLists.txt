cmake_minimum_required(VERSION 3.16)

project(GameBruh)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl3")

find_package(SDL3 REQUIRED)

add_executable(
    GameBruh

    src/main.cpp

    src/GameBoy.cpp
    src/cpu.cpp
    src/memory.cpp
    src/cartridge.cpp
    src/PPU.cpp
    src/Timer.cpp
    src/renderer.cpp
    src/debugger.cpp
    src/inputmanager.cpp
    src/joypad.cpp
    src/APU.cpp
)

target_link_libraries(
    GameBruh
    PRIVATE SDL3::SDL3
)