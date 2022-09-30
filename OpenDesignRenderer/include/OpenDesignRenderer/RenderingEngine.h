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

    /*!
        \brief Draw an image to the specified position on the frame buffer.
        \param image The image to be drawn.
        \param imagePosition The position in the frame buffer where the image will be drawn.
        \param imageDimensions The dimensions of the drawn image. May cause the image to scale.
    */
    bool Draw(
        const Image& image,
        const PixelCoordinatesUnbounded& imagePosition,
        const ImageDimensions& imageDimensions);
    /*!
        \brief Draw a rectangle to the specified position on the frame buffer.
        \param rectanglePosition The position in the frame buffer where the rectangle will be drawn.
        \param rectangleDimensions The dimensions of the drawn rectangle.
        \param fillColor The fill color of the rectangle.
        \param innerStrokeWidth Stroke width, the stroke will be rendered inside the rectange.
        \param strokeColor Stoke color.
    */
    bool DrawRectangle(
        const PixelCoordinatesUnbounded& rectanglePosition,
        const ImageDimensions& rectangleDimensions,
        const PixelColor& fillColor,
        uint32_t innerStrokeWidth,
        const PixelColor& strokeColor);

private:
    Image frameBuffer;
};
}
