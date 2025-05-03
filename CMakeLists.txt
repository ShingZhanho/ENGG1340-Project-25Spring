cmake_minimum_required(VERSION 3.14)
project(Shoot
  LANGUAGES CXX
  VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FTXUI library
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v6.0.2
)
FetchContent_MakeAvailable(ftxui)

# Project
include_directories(include)

## Core component
add_library(core
  src/core/arena_reader.cpp
  include/core/arena_reader.hpp
  src/core/arena.cpp
  include/core/arena.hpp
  include/core/entity_type.hpp
  src/core/entity.cpp
  include/core/entity.hpp
  src/core/event_handler.cpp
  include/core/event_handler.hpp
  src/core/game_options.cpp
  include/core/game_options.hpp
  src/core/game.cpp
  include/core/game.hpp
  src/core/leaderboard.cpp
  include/core/leaderboard.hpp
  include/core/point.hpp
)
target_include_directories(core PUBLIC include/core)
target_link_libraries(core PRIVATE ftxui::component ftxui::dom ftxui::screen)

## UI component
add_library(ui
  src/ui/common.cpp
  include/ui/common.hpp
  src/ui/game_ui_renderer.cpp
  include/ui/game_ui_renderer.hpp
  src/ui/render_option.cpp
  include/ui/render_option.hpp
)
target_include_directories(ui PUBLIC include/ui)
target_link_libraries(ui PRIVATE ftxui::component ftxui::dom ftxui::screen)

## UTIL component
add_library(util
  src/util/log.cpp
  include/util/log.hpp
)
target_include_directories(util PUBLIC include/util)

## Executable
add_executable(shoot
  src/main.cpp
  src/game_level_ui.cpp
  src/game_level_ui.hpp
  src/game_score_ui.cpp
  src/game_score_ui.hpp
  src/leaderboard_ui.cpp
  src/leaderboard_ui.hpp
)
target_include_directories(shoot PUBLIC include)
target_link_libraries(shoot PRIVATE core ui util ftxui::component ftxui::dom ftxui::screen)

## Copy assets
set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(ASSETS_DEST "${CMAKE_CURRENT_BINARY_DIR}/res")
add_custom_target(assets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_DIR}" "${ASSETS_DEST}"
  COMMENT "Copying game resources to output directory"
)