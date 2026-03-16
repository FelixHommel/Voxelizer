#include "./Application.hpp"

#include <spdlog/spdlog.h>

#include <exception>

int main()
{
    try
    {
        vox::app::Application app{};

        app.run();
    }
    catch(const std::exception& e)
    {
        spdlog::error(e.what());
    }

    return 0;
}

