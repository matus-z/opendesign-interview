#pragma once

#include <stdint.h>

namespace odr {
//! Image 2D dimensions.
struct ImageDimensions {
    uint32_t width;
    uint32_t height;

    uint32_t Size() const;
    uint32_t DataSize() const;

    bool operator==(const ImageDimensions& other) const;
    bool operator!=(const ImageDimensions& other) const;
};
}
