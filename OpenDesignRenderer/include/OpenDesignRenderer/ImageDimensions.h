#pragma once

namespace odr {

//! Image 2D dimensions.
struct ImageDimensions {
    uint32_t width;
    uint32_t height;

    uint32_t Size() const {
        return width * height;
    }
    uint32_t DataSize() const {
        return Size() * 4;
    }

    //! Detect if other image dimensions are identical to these.
    bool operator==(const ImageDimensions& other) const {
        return
            width == other.width &&
            height == other.height;
    }
    //! Detect if other image dimensions are different from these.
    bool operator!=(const ImageDimensions& other) const {
        return !(*this == other);
    }
};
}
