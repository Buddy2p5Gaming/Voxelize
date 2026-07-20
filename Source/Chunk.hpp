#pragma once

/* Include */
#include "Basic.hpp"
#include "Block.hpp"

// chunk
namespace Voxelize::World {
    // types
    typedef Voxelize::Length ChunkLengthProperty;
    typedef Voxelize::U16 ChunkBlockAxis;
    typedef ChunkBlockAxis ChunkBlockAxisX;
    typedef ChunkBlockAxis ChunkBlockAxisY;
    typedef ChunkBlockAxis ChunkBlockAxisZ;

    // constants
    const ChunkBlockAxis ChunkBlockDimension = 16;
    const ChunkBlockAxisX ChunkBlockLengthX = ChunkBlockDimension;
    const ChunkBlockAxisY ChunkBlockLengthY = ChunkBlockDimension;
    const ChunkBlockAxisZ ChunkBlockLengthZ = ChunkBlockDimension;
    const ChunkLengthProperty ChunkBlockCount = ChunkBlockLengthX * ChunkBlockLengthY * ChunkBlockLengthZ;

    // constants
    class Chunk {
    public:
        // blocks allocation
        Voxelize::World::Block blocks[Voxelize::World::ChunkBlockCount];

        
    };
}
