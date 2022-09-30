#pragma once

#include <OpenDesignRenderer/Image.h>


namespace odr {
//! Simple 2D rendering engine.
class RenderingEngine {
public:
    explicit RenderingEngine() = default;

    //! Initialize frame buffer to the specified dimensions.
    bool SetFrameBuffer(const ImageDimensions& dimensions);
    //! Copy frame buffer to image.
    bool CopyFrameBuffer(Image& image) const;

    //! Draw an image to the specified position on the frame buffer.
    bool Draw(
        const Image& image,
        const PixelCoordinates& position,
        const ImageDimensions& dimensions);
    //! Draw a rectangle to the specified position on the frame buffer.
    bool DrawRectangle(
        const PixelCoordinates& position,
        const ImageDimensions& dimensions,
        const PixelColor& fillColor,
        uint32_t innerStrokeWidth,
        const PixelColor& strokeColor);

private:
    Image frameBuffer;
};
}
