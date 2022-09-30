#pragma once

#include <stdint.h>

namespace odr {
//! Coordinates in 2D Euclidean space.
template<typename T>
struct Coordinates {
    T left;
    T top;
};
//! 2D pixel coordinates.
using PixelCoordinates = Coordinates<uint32_t>;
//! 2D unbounded pixel coordinated.
using PixelCoordinatesUnbounded = Coordinates<int32_t>;
}
