cmake_minimum_required(VERSION 3.25)
project(
    Voxelizer
    DESCRIPTION "Dense voxel renderer"
    LANGUAGES C CXX
    VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_definitions(VOX_ROOT="${CMAKE_SOURCE_DIR}/")

include(cmake/VcpkgDependencies.cmake)
include(cmake/DefaultProjectSettings.cmake)
include(cmake/CodeCoverage.cmake)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

enable_testing()
add_subdirectory(src)
