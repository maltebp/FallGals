set(FG_CPP_VERSION 17)

set(FG_EXTERNAL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/5-External)

add_subdirectory(src/4-Utility)
add_subdirectory(src/3-Core)
add_subdirectory(src/2-Game)
add_subdirectory(src/1-Tests)
add_subdirectory(src/1-GameApp)