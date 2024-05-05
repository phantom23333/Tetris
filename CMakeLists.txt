# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Tetris")

set(SFML BUILD_SHARED_LIBS TRUE)
add_subdirectory("thirdparty/SFML")


file(GLOB_RECURSE SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable (Tetris)
target_sources(Tetris PUBLIC ${SRC_DIR} ${INC_DIR})
target_include_directories(Tetris PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Tetris sfml-graphics)
target_compile_features(Tetris PRIVATE cxx_std_17)
add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") 

if(WIN32)
    add_custom_command(
        TARGET Tetris POST_BUILD
        COMMENT "Copying SFML DLLs"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_BINARY_DIR}/thirdparty/SFML/bin"
            $<TARGET_FILE_DIR:Tetris>
        VERBATIM
    )
endif()


