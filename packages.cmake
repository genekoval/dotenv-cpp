include(FetchContent)

if(PROJECT_TESTING)
    FetchContent_Declare(GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        b10fad38c4026a29ea6561ab15fc4818170d1c10 # branch: main
    )

    FetchContent_MakeAvailable(GTest)
endif()
