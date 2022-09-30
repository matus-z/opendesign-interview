#pragma once

#include <stdint.h>

namespace odr {
//! Image 2D dimensions.
struct ImageDimensions {
    uint32_t width;
    uint32_t height;

    //! Image size. The amount of pixels.
    uint32_t Size() const;
    //! Size of the image in bytes. This is Size()*4 since there are 4 channels per pixel (RGBA).
    uint32_t DataSize() const;

    bool operator==(const ImageDimensions& other) const;
    bool operator!=(const ImageDimensions& other) const;
};
//! ImageDimensions presets
constexpr ImageDimensions IMAGE_DIMENSIONS_EMPTY{ 0, 0 };
}
