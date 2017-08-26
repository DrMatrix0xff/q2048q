cmake_minimum_required(VERSION 2.8.11)
project(q2048q)

set(CMAKE_INCLUDE_CURRENT_DIR on)
set(CMAKE_AUTOMOC on)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

if(WIN32)
    message("platform is windows")
    add_executable(game2048 WIN32 main.cpp mainwindow.cpp tileboard.cpp)
else(WIN32)
    message("platform is not windows")
    add_executable(game2048 main.cpp mainwindow.cpp tileboard.cpp)
endif()

target_link_libraries(game2048 Qt5::Core Qt5::Widgets Qt5::Gui)

