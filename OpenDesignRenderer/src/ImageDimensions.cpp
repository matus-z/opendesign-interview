#include <OpenDesignRenderer/ImageDimensions.h>


uint32_t odr::ImageDimensions::Size() const {
    return width * height;
}

uint32_t odr::ImageDimensions::DataSize() const {
    return Size() * 4;
}

bool odr::ImageDimensions::operator==(const ImageDimensions& other) const {
    return
        width == other.width &&
        height == other.height;
}

bool odr::ImageDimensions::operator!=(const ImageDimensions& other) const {
    return !(*this == other);
}
