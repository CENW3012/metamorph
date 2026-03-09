cmake_minimum_required(VERSION 3.10)
project(HorrorGame C)

set(CMAKE_C_STANDARD 11)

find_package(SDL3 REQUIRED)

include_directories(include)

add_executable(horrorgame
    main.c
    src/game.c
    src/ui.c
    src/player.c
    src/world.c
    src/dialogue.c
    src/story.c
    src/utils.c
)

target_link_libraries(horrorgame SDL3::SDL3)
