#pragma once

// includes
#include "Basic.hpp"
#include "Block.hpp"
#include "Chunk.hpp"

namespace Voxelize::World {
    // measures in units of chunks (1 Chunk = 1 ChunkAxis)
    typedef Voxelize::U64 ChunkAxis;
    typedef ChunkAxis ChunkAxisX;
    typedef ChunkAxis ChunkAxisY;
    typedef ChunkAxis ChunkAxisZ;
    const ChunkAxis NullChunkAxisValue = 0;
    
    // one chunk based index
    class ChunkAxisCoord {
    public:
        ChunkAxisX x;
        ChunkAxisY y;
        ChunkAxisZ z;

        ChunkAxisCoord() {
            x = NullChunkAxisValue;
            y = NullChunkAxisValue;
            z = NullChunkAxisValue;
        }

        ChunkAxisCoord(ChunkAxisX _x, ChunkAxisY _y, ChunkAxisZ _z) {
            x = _x;
            y = _y;
            z = _z;
        }
    };

    // the allocation of chunks in a world
    class ChunkManager {
        
    };

    // the game state
    class WorldManager {
        // chunks
        std::vector<Voxelize::World::Chunk> chunks;

    public:
        void ChangeVisibleChunkDiameters(ChunkAxisCoord ) {

        }
    };
}
