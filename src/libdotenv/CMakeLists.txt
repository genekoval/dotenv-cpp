target_sources(dotenv
    PRIVATE
        load.cpp
        parse.cpp
)

if(PROJECT_TESTING)
    target_sources(dotenv.test
        PRIVATE
            parse.test.cpp
    )
endif()
