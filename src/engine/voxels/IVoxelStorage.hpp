#ifndef VOX_SRC_ENGINE_VOXELS_I_VOXEL_STORAGE_HPP
#define VOX_SRC_ENGINE_VOXELS_I_VOXEL_STORAGE_HPP

#include <glm/glm.hpp>

#include <cstdint>

namespace vox
{

struct VoxelSample
{
    std::uint8_t color;
    // NOTE: Can easily extend for more information later on
};

class IVoxelStorage
{
public:
    IVoxelStorage() = default;
    virtual ~IVoxelStorage() = default;

    IVoxelStorage(const IVoxelStorage&) = default;
    IVoxelStorage& operator=(const IVoxelStorage&) = default;
    IVoxelStorage(IVoxelStorage&&) noexcept = default;
    IVoxelStorage& operator=(IVoxelStorage&&) noexcept = default;

    virtual void uploadToGPU() = 0;
    virtual void bind(std::uint32_t slot = 0) = 0;
};

} // namespace vox

#endif // !VOX_SRC_ENGINE_VOXELS_I_VOXEL_STORAGE_HPP

