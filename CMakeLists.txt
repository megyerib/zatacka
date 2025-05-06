cmake_minimum_required(VERSION 3.10)
project(zatacka VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MAIN EXECUTABLE ##############################################################

add_executable(zatacka
    code/main.cpp
    code/eredmenyjelzo.cpp
    code/jatekter.cpp
    code/menu.cpp
    code/port.cpp
    code/szabalyok.cpp
    code/two_layer_drawer.cpp
    code/zatacka.cpp
)

# Add version string to the menu
execute_process (
    COMMAND version_string.sh
    OUTPUT_VARIABLE VERSION_STR
)

target_compile_definitions(zatacka
    PRIVATE VERSION="${VERSION_STR}"
)

# PACKER, RESOURCES ############################################################

add_subdirectory(packer)
add_subdirectory(res)
target_link_libraries(zatacka
    PRIVATE resource
)

# SDL 2 ########################################################################

find_package(SDL2 REQUIRED)
find_library(SDL2_GFX_LIBRARIES SDL2_gfx REQUIRED)
find_library(SDL2_IMAGE_LIBRARIES SDL2_image REQUIRED)
find_library(SDL2_TTF_LIBRARIES SDL2_ttf REQUIRED)

target_include_directories(zatacka
    PRIVATE ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(zatacka
    PRIVATE ${SDL2_LIBRARIES}
    PRIVATE ${SDL2_TTF_LIBRARIES}
    PRIVATE ${SDL2_GFX_LIBRARIES}
    PRIVATE ${SDL2_IMAGE_LIBRARIES}
)
