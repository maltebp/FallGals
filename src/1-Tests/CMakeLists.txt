IF (NOT DEFINED all_libs)
    message( FATAL_ERROR "'all_libs' variable is not defined" )
ENDIF (NOT DEFINED all_libs)


file(GLOB_RECURSE FILES
    "*.hpp"
    "*.h"
    "*.cpp"
)

add_executable(FallGals.Tests ${FILES})
set_property(TARGET FallGals.Tests PROPERTY CXX_STANDARD ${FG_CPP_VERSION})

target_link_libraries(FallGals.Tests 
    ${all_libs} 
    FallGals.Core 
    FallGals.Utility 
    FallGals.Game
)

target_include_directories(FallGals.Tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${FG_EXTERNAL_HEADERS}
)

IF (WIN32)
    
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
    file(COPY ${DLLFileList} DESTINATION .)
    
ENDIF(WIN32)