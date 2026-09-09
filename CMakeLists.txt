cmake_minimum_required(VERSION 3.20)

project(winfi
    VERSION 0.1.0
    DESCRIPTION "rofi-inspired application launcher for Windows"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#mwindows
add_executable(winfi WIN32
    src/main.c
    src/window.c
    src/input.c
    src/render.c
    src/app.c
)

target_include_directories(winfi PRIVATE src)
target_compile_definitions(winfi PRIVATE UNICODE _UNICODE)

target_link_libraries(winfi PRIVATE
    user32      #window, message loop, DrawText
    gdi32       #brushes, text color
    shell32     #shellExec
)

if(MINGW)
    target_link_options(winfi PRIVATE -municode)
    target_compile_options(winfi PRIVATE -Wall -Wextra)
endif()
