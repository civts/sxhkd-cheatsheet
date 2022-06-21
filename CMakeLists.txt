cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v2.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(sxhkd-cheatsheet
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(sxhkd-cheatsheet src/main.cpp)
target_include_directories(sxhkd-cheatsheet PRIVATE src)

target_link_libraries(sxhkd-cheatsheet
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  -static
)

install(TARGETS sxhkd-cheatsheet RUNTIME DESTINATION "bin")
