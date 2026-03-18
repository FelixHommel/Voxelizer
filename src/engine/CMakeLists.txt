set(NAME "VoxelizerEngine")
include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(${NAME}
    ./core/Camera.cpp
    ./core/Shader.cpp
    ./core/Texture.cpp
    ./external/stb_image_impl.cpp
    ./voxels/VoxelGrid.cpp
)

target_sources(${NAME}
    PRIVATE
        FILE_SET HEADERS
        FILES
            ./core/Assertions.hpp
            ./core/Camera.hpp
            ./core/Shader.hpp
            ./core/Texture.hpp
            ./core/Viewport.hpp
            ./voxels/IVoxelStorage.hpp
            ./voxels/Voxel.hpp
            ./voxels/VoxelGrid.hpp
)

target_compile_features(${NAME} PRIVATE cxx_std_23)

if(MSVC)
    target_compile_options(${NAME} PRIVATE
        /external:W0
        /external:anglebrackets
        /external:templates-
    )
endif()

target_link_libraries(${NAME}
    PUBLIC
        Glad2CMake::glad2cmake
        glfw
        glm::glm
        spdlog::spdlog
    PRIVATE
        project_warnings
        OpenGL::GL
        fmt::fmt
)
target_include_directories(${NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${Stb_INCLUDE_DIR}
)
