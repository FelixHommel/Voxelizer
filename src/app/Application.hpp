#ifndef VOX_SRC_APP_APPLICATION_HPP
#define VOX_SRC_APP_APPLICATION_HPP

#include "core/Camera.hpp"
#include "core/Shader.hpp"

#include <cstddef>
#include <cstdint>
#include <spdlog/spdlog.h>
#include <vector>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <array>
#include <filesystem>
#include <memory>
#include <string>

namespace
{

constexpr std::array<glm::ivec2, 2> CONTEXT_VERSIONS{ glm::ivec2(4, 6), glm::ivec2(4, 5) };
constexpr auto CLEAR_COLOR{ 0.2f };
constexpr auto CAMERA_START_POS{ glm::vec3(0.f, 0.f, 3.f) };

constexpr std::array<float, 180> CUBE_VERTICES{
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
};

constexpr auto NEAR_PLANE{ 0.1f };
constexpr auto FAR_PLANE{ 100.f };

struct GLFWwindowDeleter
{
    void operator()(GLFWwindow* window)
    {
        if(window != nullptr)
            glfwDestroyWindow(window);
    }
};

struct TextureInfo
{
    int width;
    int height;
    int nrChannels;
    std::vector<std::byte> pixels;
};

const auto glfwErrorCallback{ [](int error, const char* description) {
    spdlog::error("GLFW Error {}: {}", error, description);
} };

} // namespace

namespace vox::app
{

class Application
{
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void run();

private:
    std::unique_ptr<GLFWwindow, ::GLFWwindowDeleter> m_window{
        std::unique_ptr<GLFWwindow, ::GLFWwindowDeleter>(nullptr, {})
    };
    std::uint16_t m_windowWidth;
    std::uint16_t m_windowHeight;

    Camera m_camera{ ::CAMERA_START_POS };
    bool m_firstMouse{ true };
    float m_lastMouseX{ 0 };
    float m_lastMouseY{ 0 };

    void processInput(GLFWwindow* window, float deltaTime);
    static constexpr std::string readFile(const std::filesystem::path& filepath);
    static constexpr ::TextureInfo loadTexture(const std::filesystem::path& filepath);
};

} // namespace vox::app

#endif // !VOX_SRC_APP_APPLICATION_HPP

