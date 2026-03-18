#ifndef VOX_SRC_ENGINE_VOXELS_VOXEL_GRID_HPP
#define VOX_SRC_ENGINE_VOXELS_VOXEL_GRID_HPP

#include "./IVoxelStorage.hpp"
#include "./Voxel.hpp"

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace vox
{

class VoxelGrid : public IVoxelStorage
{
public:
    VoxelGrid()
        : m_voxelGrid(GRID_DIM_CUBED)
    {
        populateVoxels();
        glGenTextures(1, &m_texture);

        bind();

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, GRID_DIM, GRID_DIM, GRID_DIM, 0, GL_RGBA8, GL_UNSIGNED_BYTE, m_voxelGrid.data());

        glBindTexture(GL_TEXTURE_3D, 0);
    }

    ~VoxelGrid() override
    {
        glDeleteTextures(1, &m_texture);
    }

    VoxelGrid(const VoxelGrid&) = delete;
    VoxelGrid& operator=(const VoxelGrid&) = delete;
    VoxelGrid(VoxelGrid&&) noexcept = delete;
    VoxelGrid& operator=(VoxelGrid&&) noexcept = delete;

    [[nodiscard]] static constexpr glm::vec3 gridDimensions() noexcept { return { GRID_DIM, GRID_DIM, GRID_DIM }; }

    void uploadToGPU() override
    {
        // FIXME: Implement this method
        // NOTE: Not really necessary I think. GPU upload is done in the constructor with glTexImage3D(...)
    }

    void bind(std::uint32_t slot = 0) override
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_3D, m_texture);
    }

private:
    static constexpr std::size_t GRID_DIM{ 64 };
    static constexpr std::size_t GRID_DIM_CUBED{ GRID_DIM * GRID_DIM * GRID_DIM };
    static constexpr std::uint8_t BLACK_VOXEL{ 255 };

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

    void populateVoxels()
    {
        // NOTE: Make the bottom layer of the voxel grid black, to verify presence (Implication: color = 0 => no voxel present).
        for(auto i{ 0 }; i < GRID_DIM; ++i)
        {
            for(auto j{ 0 }; j < GRID_DIM; ++j)
            {
                auto& v{ voxelAt(i, 0, j) };

                v.colorR = BLACK_VOXEL;
                v.colorG = BLACK_VOXEL;
                v.colorB = BLACK_VOXEL;
                v.colorA = BLACK_VOXEL;
            }
        }
    }
};

} // namespace vox

#endif // !VOX_SRC_ENGINE_VOXELS_VOXEL_GRID_HPP

