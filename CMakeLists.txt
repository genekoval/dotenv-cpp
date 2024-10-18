cmake_minimum_required(VERSION 3.26)

include(cmake/modules.cmake)
include(GitVersion)

project(dotenv
    VERSION ${GIT_VERSION_ABBREV}
    DESCRIPTION "Loads environment variables from an .env file"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

include(ProjectTesting)
include(packages.cmake)

add_library(dotenv "")
add_library(dotenv::dotenv ALIAS dotenv)

target_sources(dotenv PUBLIC FILE_SET HEADERS BASE_DIRS include)

if(PROJECT_TESTING)
    add_executable(dotenv.test "")

    target_link_libraries(dotenv.test PRIVATE
        dotenv
        GTest::gtest_main
    )

    add_test(
        NAME "Unit Tests"
        COMMAND dotenv.test
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
endif()

add_subdirectory(include)
add_subdirectory(src)

include(Sanitizers)
enable_asan()

include(EasyInstall)
easy_install(TARGETS dotenv)

if(PROJECT_IS_TOP_LEVEL)
    add_custom_target(docker
        docker build --progress=plain --no-cache .
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
    )
endif()
