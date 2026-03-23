#ifndef VOX_SRC_ENGINE_VOXELS_VOXEL_GRID_HPP
#define VOX_SRC_ENGINE_VOXELS_VOXEL_GRID_HPP

#include "./IVoxelStorage.hpp"
#include "./Voxel.hpp"

#include "external/PerlinNoise.hpp"
#include <glad/gl.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace vox
{

/// \brief A \ref VoxelGrid is a simple 3D container of voxels where each element corresponds to one voxel.
///
/// \author Felix Hommel
/// \date 3/20/2026
class VoxelGrid : public IVoxelStorage
{
public:
    VoxelGrid();
    ~VoxelGrid() override;

    VoxelGrid(const VoxelGrid&) = delete;
    VoxelGrid& operator=(const VoxelGrid&) = delete;
    VoxelGrid(VoxelGrid&&) noexcept = delete;
    VoxelGrid& operator=(VoxelGrid&&) noexcept = delete;

    [[nodiscard]] static constexpr glm::vec3 gridDimensionsMin() noexcept { return { 0, 0, 0 }; }
    [[nodiscard]] static constexpr glm::vec3 gridDimensionsMax() noexcept { return { GRID_DIM, GRID_DIM, GRID_DIM }; }

    void uploadToGPU() override;
    void bind(std::uint32_t slot = 0) override;

private:
    static constexpr std::size_t GRID_DIM{ 64 };
    static constexpr std::size_t GRID_DIM_CUBED{ GRID_DIM * GRID_DIM * GRID_DIM };
    static constexpr std::uint8_t WHITE_VOXEL{ 255 };
    static constexpr siv::PerlinNoise::seed_type PERLIN_SEED{ 123456u };
    static constexpr auto PERLIN_VALUE_FACTOR{ 0.01f };
    static constexpr std::int32_t PERLIN_OCTAVES{ 4 };

    const siv::PerlinNoise perlin;

    std::vector<Voxel> m_voxelGrid;
    std::uint32_t m_texture{ 0 };

    [[nodiscard]] Voxel& voxelAt(std::size_t x, std::size_t y, std::size_t z)
    {
        return m_voxelGrid.at(x + (y * GRID_DIM) + (z * GRID_DIM * GRID_DIM));
    }

    [[nodiscard]] const Voxel& voxelAt(std::size_t x, std::size_t y, std::size_t z) const
    {
        return m_voxelGrid.at(x + (y * GRID_DIM) + (z * GRID_DIM * GRID_DIM));
    }

    void populateVoxels();
};

} // namespace vox

#endif // !VOX_SRC_ENGINE_VOXELS_VOXEL_GRID_HPP

