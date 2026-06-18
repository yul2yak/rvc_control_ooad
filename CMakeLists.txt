cmake_minimum_required(VERSION 3.14)
project(rvc_sw_controller LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RVC_ENABLE_COVERAGE "Enable gcov coverage flags" OFF)
option(RVC_BUILD_SIMULATOR "Build GUI simulator (requires SFML)" ON)
option(RVC_BUILD_SYSTEM_TEST "Build system test runner" ON)

if(RVC_ENABLE_COVERAGE)
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

add_library(rvc_core
    src/rvc/MapModel.cpp
    src/rvc/CleaningService.cpp
    src/rvc/MovementController.cpp
    src/rvc/ObstacleDetector.cpp
    src/rvc/DustDetector.cpp
    src/rvc/RvcController.cpp
    src/rvc/MapJson.cpp
    src/rvc/SimulationEngine.cpp
)
target_include_directories(rvc_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(rvc_unit_tests
    test/cleaning_service_test.cpp
    test/movement_controller_test.cpp
    test/rvc_controller_test.cpp
    test/simulation_engine_test.cpp
)
target_link_libraries(rvc_unit_tests PRIVATE rvc_core GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(rvc_unit_tests)

if(RVC_BUILD_SYSTEM_TEST)
    add_executable(rvc_system_test
        System-Test/system_test_main.cpp
    )
    target_link_libraries(rvc_system_test PRIVATE rvc_core)
endif()

if(RVC_BUILD_SIMULATOR)
    if(NOT SFML_DIR AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SFML/lib/cmake/SFML/SFMLConfig.cmake")
        set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SFML/lib/cmake/SFML" CACHE PATH "SFML cmake config")
    endif()
    find_package(SFML 2.5 COMPONENTS graphics window system QUIET)
    if(SFML_FOUND)
        add_executable(rvc_sim
            sim/main.cpp
            sim/SimulatorApp.cpp
        )
        target_include_directories(rvc_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sim)
        target_link_libraries(rvc_sim PRIVATE rvc_core sfml-graphics sfml-window sfml-system)
        if(MSVC)
            target_compile_options(rvc_sim PRIVATE /utf-8)
        endif()
        if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SFML/bin")
            add_custom_command(TARGET rvc_sim POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SFML/bin"
                    "$<TARGET_FILE_DIR:rvc_sim>"
                COMMENT "Copy SFML runtime DLLs next to rvc_sim")
        endif()
    else()
        message(WARNING "SFML not found — rvc_sim GUI target skipped. Install SFML or set SFML_DIR.")
    endif()
endif()
