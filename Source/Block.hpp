#pragma once

/* Includes */
#include "Basic.hpp"

// namespace
namespace Voxelize::World {
    /* One Block */
    // block internals
    typedef Voxelize::U16 SolidBlockID;

    // one block
    class Block {
    public:
        SolidBlockID solidBlockID;
    };
}
