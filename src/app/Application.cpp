#include "Application.hpp"

#include "core/Shader.hpp"
#include "voxels/VoxelGrid.hpp"

#include <fmt/format.h>
#include <glad/gl.h>
#include <glm/ext/matrix_clip_space.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace vox::app
{

Application::Application()
{
    glfwSetErrorCallback(::glfwErrorCallback);

    if(glfwInit() == GLFW_FALSE)
    {
        spdlog::info("Failed to initialize GLFW");
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ::CONTEXT_VERSIONS[0].x);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ::CONTEXT_VERSIONS[0].y);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GL_TRUE);
#endif

    auto* m{ glfwGetPrimaryMonitor() };
    const auto* videoMode{ glfwGetVideoMode(m) };

    m_windowWidth = static_cast<std::uint16_t>(videoMode->width);
    m_windowHeight = static_cast<std::uint16_t>(videoMode->height);
    m_window.reset(glfwCreateWindow(videoMode->width, videoMode->height, "Voxelizer", nullptr, nullptr));

    if(m_window == nullptr)
    {
        spdlog::info("Could not open a GLFW window with OpenGL context version 4.6, trying with 4.5");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ::CONTEXT_VERSIONS[1].x);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ::CONTEXT_VERSIONS[1].y);
        m_window.reset(glfwCreateWindow(videoMode->width, videoMode->height, "Voxeler", nullptr, nullptr));

        if(m_window == nullptr)
        {
            spdlog::info("Failed to create a GLFW window with 4.5 or 4.6 context");
            glfwTerminate();
            throw std::runtime_error("Failed to create a GLFW window with a valid context (tested 4.5 and 4.6)");
        }
    }

    glfwMakeContextCurrent(m_window.get());
    glfwSetWindowUserPointer(m_window.get(), this);
    glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height) {
        auto* self{ static_cast<Application*>(glfwGetWindowUserPointer(window)) };

        glViewport(0, 0, width, height);
        self->m_windowWidth = static_cast<std::uint16_t>(width);
        self->m_windowHeight = static_cast<std::uint16_t>(height);
    });
    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double posXIn, double posYIn) {
        auto* self{ static_cast<Application*>(glfwGetWindowUserPointer(window)) };

        float posX{ static_cast<float>(posXIn) };
        float posY{ static_cast<float>(posYIn) };

        if(self->m_firstMouse)
        {
            self->m_lastMouseX = posX;
            self->m_lastMouseY = posY;
            self->m_firstMouse = false;
        }

        float offsetX{ posX - self->m_lastMouseX };
        float offsetY{ posY - self->m_lastMouseY };

        self->m_lastMouseX = posX;
        self->m_lastMouseY = posY;

        self->m_camera.processMouseMovement(offsetX, offsetY);
    });
    glfwSetScrollCallback(m_window.get(), [](GLFWwindow* window, double, double offsetY) {
        auto* self{ static_cast<Application*>(glfwGetWindowUserPointer(window)) };

        self->m_camera.porocessMouseScroll(static_cast<float>(offsetY));
    });

    if(gladLoadGL(glfwGetProcAddress) == 0)
    {
        spdlog::info("Failed to initialize GLAD");
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // NOTE: Prepare OpenGL state
    glEnable(GL_DEPTH_TEST);
}

Application::~Application()
{
    m_window.reset(nullptr);
    glfwTerminate();
}

void Application::run()
{
    auto shader{ std::make_unique<Shader>(
        readFile(VOX_ROOT "resources/shaders/voxelDDA_simple.vert").c_str(),
        readFile(VOX_ROOT "resources/shaders/voxelDDA_simple.frag").c_str()
    ) };

    shader->use();
    shader->setInteger("voxelGrid", 0);

    VoxelGrid grid{};

    float deltaTime{ 0.f };
    float lastTime{ 0.f };
    while(glfwWindowShouldClose(m_window.get()) == GLFW_FALSE)
    {
        float currentTime{ static_cast<float>(glfwGetTime()) };
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(m_window.get(), deltaTime);

        glClearColor(0.f, CLEAR_COLOR, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
        grid.bind(0);

        auto projection{ glm::perspective(
            glm::radians(m_camera.zoom()),
            static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight),
            ::NEAR_PLANE,
            ::FAR_PLANE
        ) };
        auto invViewProj{ glm::inverse(projection * m_camera.viewMatrix()) };

        shader->setMatrix4("invViewProj", invViewProj);
        shader->setVector3f("cameraPos", m_camera.position());
        shader->setVector2f("resolution", static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight));

        // NOLINTNEXTLINE(readability-magic-numbers): 3 vertices for one simple triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(m_window.get());
        glfwPollEvents();
    }
}

void Application::processInput(GLFWwindow* window, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.processKeyboard(Camera::CameraMovement::FORWARD, deltaTime);
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.processKeyboard(Camera::CameraMovement::BACKWARD, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.processKeyboard(Camera::CameraMovement::LEFT, deltaTime);
    else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.processKeyboard(Camera::CameraMovement::RIGHT, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera.processKeyboard(Camera::CameraMovement::UP, deltaTime);
    else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_camera.processKeyboard(Camera::CameraMovement::DOWN, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

constexpr std::string Application::readFile(const std::filesystem::path& filepath)
{
    if(!std::filesystem::exists(filepath))
        throw std::runtime_error(fmt::format("File '{}' does not exist", filepath.string()));

    std::ifstream file(filepath);
    if(!file)
        throw std::runtime_error(fmt::format("File '{}' could not be opened", filepath.string()));

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

constexpr ::TextureInfo Application::loadTexture(const std::filesystem::path& filepath)
{
    if(!std::filesystem::exists(filepath))
        throw std::runtime_error(fmt::format("Provided texture '{}' does not exist", filepath.string()));

    int width{};
    int height{};
    int nrChannels{};
    // NOTE: Need to access filepath as u8string to ensure windows file paths are accessed in the right way
    stbi_uc* imageData{
        stbi_load(reinterpret_cast<const char*>(filepath.u8string().c_str()), &width, &height, &nrChannels, 0)
    };

    if(imageData == nullptr)
        throw std::runtime_error(fmt::format("stb_image failed: {}", stbi_failure_reason()));

    if(width <= 0 || height <= 0 || nrChannels <= 0)
    {
        stbi_image_free(imageData);
        throw std::runtime_error(
            fmt::format("Invalid image dimensions (width: {}, height: {}, channels: {})", width, height, nrChannels)
        );
    }

    const auto w{ static_cast<std::size_t>(width) };
    const auto h{ static_cast<std::size_t>(height) };
    const auto c{ static_cast<std::size_t>(nrChannels) };

    if(w > std::numeric_limits<std::size_t>::max() / h || w * h > std::numeric_limits<std::size_t>::max() / c)
    {
        stbi_image_free(imageData);
        throw std::runtime_error(
            fmt::format("Image size overflow (width: {}, height: {}, channels: {})", width, height, nrChannels)
        );
    }

    const auto size{ w * h * c };

    std::vector<std::byte> pixels(size);
    std::memcpy(pixels.data(), imageData, size);
    stbi_image_free(imageData);

    return { .width = width, .height = height, .nrChannels = nrChannels, .pixels = std::move(pixels) };
}

} // namespace vox::app

