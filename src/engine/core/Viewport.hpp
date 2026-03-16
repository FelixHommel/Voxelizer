#ifndef VOX_SRC_ENGINE_CORE_VIEWPORT_HPP
#define VOX_SRC_ENGINE_CORE_VIEWPORT_HPP

#include <cstdint>

namespace vox
{

/// \brief The \ref Viewport is used to describe the size of the opened window.
///
/// \author Felix Hommel
/// \date 3/15/2026
struct Viewport
{
    std::uint16_t width;
    std::uint16_t height;
};

} // namespace vox

#endif // !VOX_SRC_ENGINE_CORE_VIEWPORT_HPP

