set(NAME "VoxelizerTests")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_executable(${NAME}
    ./core/ShaderTest.cpp
    ./core/TextureTest.cpp
    ./testMain.cpp
    ./testUtility/stb_image_write_impl.cpp
)

target_sources(${NAME}
    PRIVATE
        FILE_SET HEADERS
        FILES
            ./fixtures/OpenGLTestFixture.hpp
            ./testUtility/RandomNumberGenerator.hpp
            ./testUtility/ResourceGenerator.hpp
)

target_compile_features(${NAME} PRIVATE cxx_std_23)
target_link_libraries(${NAME}
    PRIVATE
        project_warnings
        GTest::gtest
        GTest::gmock
        VoxelizerEngine
)

include(GoogleTest)
gtest_discover_tests(${NAME})

# NOTE: https://discourse.cmake.org/t/differences-in-set-property-vs-set-tests-properties-for-discovered-gtest-tests/8080
set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/labelTests.cmake)
