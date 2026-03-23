# Voxelizer

![Build](https://github.com/FelixHommel/Voxeler/actions/workflows/build.yml/badge.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/d688f79815f741f091938fb6b76afe46)](https://app.codacy.com/gh/FelixHommel/Voxelizer/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Codacy Badge](https://app.codacy.com/project/badge/Coverage/d688f79815f741f091938fb6b76afe46)](https://app.codacy.com/gh/FelixHommel/Voxelizer/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_coverage)

This is an attempt to create a Voxel Renderer from scratch using C++ and OpenGL. I will limit the use of external
dependencies besides some very common (i.e., [spdlog](https://github.com/gabime/spdlog), [fmt](https://github.com/fmtlib/fmt))
or very convenient (i.e., [GLFW](https://github.com/glfw/glfw), [GLM](https://github.com/g-truc/glm)) ones.

![Flat Shaded Render of a Voxel Grid](./docs/resources/flat_shaded_voxel_grid.png)

## Full List of Dependencies

- [fmt](https://github.com/fmtlib/fmt)
- [Glad2-CMake](https://github.com/FelixHommel/Glad2-CMake) (CMake compatible [Glad2](https://github.com/Dav1dde/glad) port, made by myself)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [GTest](https://github.com/google/googletest)
- [spdlog](https://github.com/gabime/spdlog)
- [stb](https://github.com/nothings/stb)
- [PerlinNoise](https://github.com/Reputeless/PerlinNoise)

## Acknowledgments / Credits

- [LearnOpenGL.com](https://learnopengl.com/)
