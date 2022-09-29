#pragma once

#include <string>

#include <OpenDesignRenderer/ImageDimensions.h>


// Forward declarations
namespace odr {
struct PixelColor;
struct PixelCoordinates;
}

namespace odr {
//! 2D Image representation.
class Image {
public:
    //! Construct an unitialized image.
    Image();
    //! Destructor.
    ~Image();

    //! Detect if the image is initialized (has data and non-null dimensions).
    bool IsInitialized() const;

    //! Clear the image - return to uninitialized state.
    void Clear();
    //! Initialize an image with the specified dimensions and color.
    bool Initialize(const ImageDimensions& dimensions, const PixelColor& color);
    //! Initialize - perform a deep copy of the other image and its data.
    bool Initialize(const Image& otherImage);

    //! Load image from an RGBA file on the filesystem.
    bool Load(const std::string& filepath);
    //! Save image to an RGBA file on the filesystem.
    bool Save(const std::string& filepath) const;

    //! Provide read-only access to image dimensions.
    const ImageDimensions& GetDimensions() const;

    //! Get color at pixel.
    PixelColor GetColor(const PixelCoordinates& coords) const;
    //! Set color at pixel.
    bool SetColor(const PixelColor& color, const PixelCoordinates& coords);

    //! Detect if another image is identical to this one.
    bool operator==(const Image& other) const;

private:
    //! Image dimensions - width and height.
    ImageDimensions dimensions;
    //! Image data buffer.
    unsigned char* imageBuffer = nullptr;
};
}
