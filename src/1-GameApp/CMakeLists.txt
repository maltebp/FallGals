file(GLOB_RECURSE FILES
    "*.hpp"
    "*.h"
    "*.cpp"
)

add_executable(FallGals.GameApp ${FILES} )
set_property(TARGET FallGals.GameApp PROPERTY CXX_STANDARD ${FG_CPP_VERSION})
set_property(TARGET FallGals.GameApp PROPERTY OUTPUT_NAME "FallGals")

target_link_libraries(FallGals.GameApp 
    ${all_libs} 
    FallGals.Core 
    FallGals.Utility 
    FallGals.Game
)

target_include_directories(FallGals.GameApp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${FG_EXTERNAL_HEADERS}
)

file(COPY ../../assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/assets)
file(COPY ../../assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/assets)

# Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug or /Release
set_target_properties(FallGals.GameApp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:FallGals.GameApp>)

IF (WIN32)
    
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
    file(COPY ${DLLFileList} DESTINATION .)

ENDIF(WIN32)