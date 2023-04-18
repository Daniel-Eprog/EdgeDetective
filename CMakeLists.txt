cmake_minimum_required(VERSION 3.21)
project(EdgeDetective LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(EdgeDetective src/main.cpp)
target_link_libraries(EdgeDetective PRIVATE sfml-graphics)
target_compile_features(EdgeDetective PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET EdgeDetective POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:EdgeDetective> $<TARGET_FILE_DIR:EdgeDetective> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS EdgeDetective)

