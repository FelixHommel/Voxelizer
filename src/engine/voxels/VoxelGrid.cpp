#include "VoxelGrid.hpp"

#include "core/Assertions.hpp"

#include "external/PerlinNoise.hpp"
#include <glad/gl.h>

#include <cstddef>
#include <cstdint>

namespace vox
{

VoxelGrid::VoxelGrid()
    : m_voxelGrid(GRID_DIM_CUBED)
    , perlin{ PERLIN_SEED }
{
    populateVoxels();
    glGenTextures(1, &m_texture);

    bind();

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage3D(
        GL_TEXTURE_3D, 0, GL_RGBA8, GRID_DIM, GRID_DIM, GRID_DIM, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_voxelGrid.data()
    );

    glBindTexture(GL_TEXTURE_3D, 0);
}

VoxelGrid::~VoxelGrid()
{
    glDeleteTextures(1, &m_texture);
}

void VoxelGrid::uploadToGPU()
{
    // FIXME: Implement this method
    // NOTE: Not really necessary I think. GPU upload is done in the constructor with glTexImage3D(...)

    VOX_ASSERT(true, "VoxelGrid does not need extra uploading to the GPU");
}

void VoxelGrid::bind(std::uint32_t slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_3D, m_texture);
}

void VoxelGrid::populateVoxels()
{
    // NOTE: Make the bottom layer of the voxel grid white, to verify presence (Implication: color = 0 => no voxel present).
    for(std::size_t y{ 0 }; y < GRID_DIM; ++y)
    {
        for(std::size_t z{ 0 }; z < GRID_DIM; ++z)
        {
            for(std::size_t x{ 0 }; x < GRID_DIM; ++x)
            {
                auto& v{ voxelAt(y, x, z) };

                const auto perlinValue{
                    perlin.octave3D_01(
                        (static_cast<float>(x) * PERLIN_VALUE_FACTOR),
                        (static_cast<float>(y) * PERLIN_VALUE_FACTOR),
                        (static_cast<float>(z) * PERLIN_VALUE_FACTOR),
                        PERLIN_OCTAVES
                    )
                };

                const auto colorValue{ static_cast<std::uint8_t>(perlinValue * WHITE_VOXEL) };

                if(perlinValue >= 0.5)
                {
                    v.colorR = colorValue;
                    v.colorG = colorValue;
                    v.colorB = colorValue;
                    v.colorA = WHITE_VOXEL;
                }
                else
                {
                    v.colorR = 0;
                    v.colorG = 0;
                    v.colorB = 0;
                    v.colorA = WHITE_VOXEL;
                }
            }
        }
    }
}

} // namespace vox

