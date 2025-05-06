cmake_minimum_required(VERSION 3.10)
project(zatacka-resources VERSION 1.0)

get_filename_component(RES_DIR ${CMAKE_SOURCE_DIR}/res ABSOLUTE)

set(RES_FILES
    ${RES_DIR}/death.png
    ${RES_DIR}/logo.png
    ${RES_DIR}/font_hun.ttf
    ${RES_DIR}/font_bold_hun.ttf
)

add_custom_command(
    OUTPUT resource.c resource.h
    COMMAND packer ${RES_FILES}
    DEPENDS packer ${RES_FILES}
)

add_custom_target(generated_header DEPENDS resource.h)

add_library(resource
    STATIC
    resource.c
)

add_dependencies(resource generated_header)

target_include_directories(resource
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)
