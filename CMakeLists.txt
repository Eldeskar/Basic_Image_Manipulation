cmake_minimum_required(VERSION 3.18)

project(Basic_Image_Manipulation VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# --------------------------------------------------------------------------------
# Loads and refreshes git submodules from git
# --------------------------------------------------------------------------------

find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules durring build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule add https://github.com/libigl/eigen dependencies/eigen
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule add https://github.com/opencv/opencv dependencies/opencv
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please update submodules and try again.")
        endif()
    endif()
endif()

# --------------------------------------------------------------------------------
# Gets all files stored in the inc and src folder of the project and stores them in lists that can be used later in the build
# --------------------------------------------------------------------------------

list(APPEND SRC_FILES "")
list(APPEND INC_FILES "")
list(APPEND TEST_SRC_FILES "")
list(APPEND TEST_INC_FILES "")

if(WIN32)
    message(STATUS "Looking for files in the following directory: ${CMAKE_CURRENT_SOURCE_DIR}")


    execute_process (
        COMMAND cmd "/C" "dir" "src\\*" "/b"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND SRC_FILES ${OUTPUT})
    list(TRANSFORM SRC_FILES PREPEND "src/")


    execute_process (
        COMMAND cmd "/C" "dir" "inc\\*" "/b"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND INC_FILES ${OUTPUT})
    list(TRANSFORM INC_FILES PREPEND "inc/")


    execute_process (
        COMMAND cmd "/C" "dir" "src\\*" "/b"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND TEST_SRC_FILES ${OUTPUT})
    list(TRANSFORM TEST_SRC_FILES PREPEND "test/src/")


    execute_process (
        COMMAND cmd "/C" "dir" "inc\\*" "/b"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND TEST_INC_FILES ${OUTPUT})
    list(TRANSFORM TEST_INC_FILES PREPEND "test/inc/")


    message(STATUS "The following files were found:")
    message(STATUS "SRC files: ${SRC_FILES}")
    message(STATUS "INC files: ${INC_FILES}")
    message(STATUS "TEST SRC files: ${TEST_SRC_FILES}")
    message(STATUS "TEST INC files: ${TEST_INC_FILES}")
ELSEIF(UNIX)
    message(STATUS "Looking for files in the following directory: ${CMAKE_CURRENT_SOURCE_DIR}")

    execute_process (
        COMMAND "ls" "-a" "inc"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND INC_FILES ${OUTPUT})
    list(TRANSFORM INC_FILES PREPEND "inc/")


    execute_process (
        COMMAND "ls" "-a" "src"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND SRC_FILES ${OUTPUT})
    list(TRANSFORM SRC_FILES PREPEND "src/")


    execute_process (
        COMMAND "ls" "-a" "inc"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND TEST_INC_FILES ${OUTPUT})
    list(TRANSFORM TEST_INC_FILES PREPEND "test/inc/")


    execute_process (
        COMMAND "ls" "-a" "src"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test"
        OUTPUT_VARIABLE OUTPUT
        COMMAND_ECHO STDOUT
    )
    string(REPLACE "\n" ";" OUTPUT ${OUTPUT})
    list(APPEND TEST_SRC_FILES ${OUTPUT})
    list(TRANSFORM TEST_SRC_FILES PREPEND "test/src/")


    message(STATUS "The following files were found:")
    message(STATUS "SRC files: ${SRC_FILES}")
    message(STATUS "INC files: ${INC_FILES}")
    message(STATUS "TEST SRC files: ${TEST_SRC_FILES}")
    message(STATUS "TEST INC files: ${TEST_INC_FILES}")
ELSE()
    message(FATAL_ERROR "Plattform not supported by this project!")
ENDIF()

# --------------------------------------------------------------------------------
# Adds google test to project
# --------------------------------------------------------------------------------

include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/master.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# --------------------------------------------------------------------------------
# Adds openMP  to the project 
# --------------------------------------------------------------------------------

set(OPENMP_LIB "")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(OPENMP_LIB OpenMP::OpenMP_CXX FORCE)
endif()

# --------------------------------------------------------------------------------
# Adds eigen to the project 
# --------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build the static library" OFF)
option(WITH_OPENMP "Enabling OPENMP" ON)

set(EIGEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/eigen" CACHE PATH "Stored the path to the eigen directory")
set(EIGEN_INC "${EIGEN_DIR}/include")

add_subdirectory("${EIGEN_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/dependencies/eigen")

# --------------------------------------------------------------------------------
# Adds opencv to the project 
# --------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build the static library" OFF)
option(WITH_OPENMP "Enabling OPENMP" ON)

set(OPENCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opencv" CACHE PATH "Stored the path to the opencv directory")
set(OPENCV_INC "${OPENCV_DIR}/include")

add_subdirectory("${OPENCV_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/dependencies/opencv")

# --------------------------------------------------------------------------------
# Defining the names of the test instance
# --------------------------------------------------------------------------------

set(TEST_INSTANCE "${PROJECT_NAME}_TEST")

# --------------------------------------------------------------------------------
# Sets up a target for the test instance
# --------------------------------------------------------------------------------

add_executable(${TEST_INSTANCE})

# --------------------------------------------------------------------------------
# Defining the cpp standard of the test environement
# --------------------------------------------------------------------------------

set_property(TARGET ${TEST_INSTANCE} PROPERTY CXX_STANDARD 17)

# --------------------------------------------------------------------------------
# Adds all sources of the project to the test target -> apparently headers dont need to be added here but if they are added they will be visible in vs project.
# --------------------------------------------------------------------------------

target_sources(${TEST_INSTANCE}
    PUBLIC
        ${INC_FILES}
        ${SRC_FILES}
        ${TEST_INC_FILES}
        ${TEST_SRC_FILES}
)

# --------------------------------------------------------------------------------
# Adds headers to the test target
# --------------------------------------------------------------------------------

target_include_directories(${TEST_INSTANCE}
    PUBLIC
        inc
        test/inc
        ${OPENCV_INC}
)

# --------------------------------------------------------------------------------
# Links juce, tensorflow and google test libraries to the test target
# --------------------------------------------------------------------------------

target_link_libraries(${TEST_INSTANCE}
    PRIVATE
        ${OpenCV_LIBS}
        ${OPENMP_LIB}
        Eigen3::Eigen
    PUBLIC
        GTest::gtest_main
)

# --------------------------------------------------------------------------------
# needed to configure ctest and add the created tests
# --------------------------------------------------------------------------------

include(GoogleTest)
gtest_discover_tests(${TEST_INSTANCE})

# --------------------------------------------------------------------------------
# Defining the names of the prod instance
# --------------------------------------------------------------------------------

set(PROD_INSTANCE "${PROJECT_NAME}_PROD")

# --------------------------------------------------------------------------------
# Sets up a target for the production instance
# --------------------------------------------------------------------------------

add_executable(${PROD_INSTANCE})

# --------------------------------------------------------------------------------
# Defining the cpp standard of the prod environement
# --------------------------------------------------------------------------------

set_property(TARGET ${PROD_INSTANCE} PROPERTY CXX_STANDARD 17)

# --------------------------------------------------------------------------------
# Adds all sources of the project to the production target -> apparently headers dont need to be added here but if they are added they will be visible in vs project.
# --------------------------------------------------------------------------------

target_sources(${PROD_INSTANCE}
    PUBLIC
        ${INC_FILES}
        ${SRC_FILES}
)

# --------------------------------------------------------------------------------
# Adds juce compile definitions to the production target
# --------------------------------------------------------------------------------

target_compile_definitions(${PROD_INSTANCE}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

# --------------------------------------------------------------------------------
# Adds headers to the production target
# --------------------------------------------------------------------------------

target_include_directories(${PROD_INSTANCE}
PUBLIC
    inc
    ${OPENCV_INC}
)


# --------------------------------------------------------------------------------
# Links juce, tensorflow and google test libraries to the production target
# --------------------------------------------------------------------------------

target_link_libraries(${PROD_INSTANCE}
    PRIVATE
        ${OpenCV_LIBS}
        ${OPENMP_LIB}
        Eigen3::Eigen
    PUBLIC
        GTest::gtest_main
)

# --------------------------------------------------------------------------------
# Copy the config.json file to the build directory
# --------------------------------------------------------------------------------

add_custom_command(TARGET ${PROD_INSTANCE} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/data/ ${CMAKE_CURRENT_BINARY_DIR}/data/)
