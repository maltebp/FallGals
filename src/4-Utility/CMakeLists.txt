IF (NOT DEFINED all_libs)
    message( FATAL_ERROR "'all_libs' variable is not defined" )
ENDIF (NOT DEFINED all_libs)

file(GLOB_RECURSE FILES
    "*.hpp"
    "*.h"
    "*.cpp"
)

add_library(FallGals.Utility STATIC ${FILES})
set_property(TARGET FallGals.Utility PROPERTY CXX_STANDARD ${FG_CPP_VERSION})

target_link_libraries(FallGals.Utility ${all_libs})

target_include_directories(FallGals.Utility
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${FG_EXTERNAL_HEADERS}
)
