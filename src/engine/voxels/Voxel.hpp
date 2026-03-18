#ifndef VOX_SRC_ENGINE_VOXELS_VOXEL_HPP
#define VOX_SRC_ENGINE_VOXELS_VOXEL_HPP

#include <cstdint>

namespace vox
{

struct Voxel
{
    std::uint8_t colorR;
    std::uint8_t colorG;
    std::uint8_t colorB;
    std::uint8_t colorA;
};

} // namespace vox

#endif // !VOX_SRC_ENGINE_VOXELS_VOXEL_HPP

