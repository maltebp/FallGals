IF (NOT DEFINED all_libs)
    message( FATAL_ERROR "'all_libs' variable is not defined" )
ENDIF (NOT DEFINED all_libs)


file(GLOB_RECURSE FILES
    "*.hpp"
    "*.h"
    "*.cpp"
)

add_library(FallGals.Game ${FILES})
set_property(TARGET FallGals.Game PROPERTY CXX_STANDARD ${FG_CPP_VERSION})

target_link_libraries(FallGals.Game ${all_libs} FallGals.Core FallGals.Utility)

target_include_directories(FallGals.Game
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${FG_EXTERNAL_HEADERS}
)