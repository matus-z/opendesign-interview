#include <OpenDesignRenderer/RenderingEngine.h>

#include <algorithm>

#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>


bool odr::RenderingEngine::InitializeFrameBuffer(const ImageDimensions& dimensions) {
    return frameBuffer.Initialize(dimensions, COLOR_TRANSPARENT);
}

bool odr::RenderingEngine::Render(Image& image) const {
    return image.CloneFrom(frameBuffer);
}

bool odr::RenderingEngine::Draw(
    const Image& image,
    const PixelCoordinatesUnbounded& imagePosition,
    const ImageDimensions& imageDimensions) {
    const Image scaledImage = image.Scaled(imageDimensions);

    const ImageDimensions& fbDimensions = frameBuffer.GetDimensions();

    const uint32_t fbLeft = std::max(imagePosition.left, 0);
    const uint32_t fbRight = std::min(imageDimensions.width + imagePosition.left, fbDimensions.width);
    const uint32_t fbTop = std::max(imagePosition.top, 0);
    const uint32_t fbBottom = std::min(imageDimensions.height + imagePosition.top, fbDimensions.height);

    bool areAllColorsSet = true;

    // For each pixel in the frame buffer where the image will be drawn
    for (uint32_t y = fbTop; y < fbBottom; y++) {
        for (uint32_t x = fbLeft; x < fbRight; x++) {
            const PixelCoordinates imgCoords{ x - imagePosition.left, y - imagePosition.top };
            const PixelCoordinates fbCoords{ x, y };

            const PixelColor imageColor = scaledImage.GetColor(imgCoords);
            const PixelColor frameBufferColor = frameBuffer.GetColor(fbCoords);
            const PixelColor blendedPixelColor = PixelColor::Blend(frameBufferColor, imageColor);

            areAllColorsSet &= frameBuffer.SetColor(blendedPixelColor, fbCoords);
        }
    }

    return areAllColorsSet;
}

bool odr::RenderingEngine::DrawRectangle(
    const PixelCoordinatesUnbounded& rectanglePosition,
    const ImageDimensions& rectangleDimensions,
    const PixelColor& fillColor,
    uint32_t innerStrokeWidth,
    const PixelColor& strokeColor) {
    const ImageDimensions& fbDimensions = frameBuffer.GetDimensions();

    const uint32_t fbLeft = std::max(rectanglePosition.left, 0);
    const uint32_t fbRight = std::min(rectangleDimensions.width + rectanglePosition.left, fbDimensions.width);
    const uint32_t fbTop = std::max(rectanglePosition.top, 0);
    const uint32_t fbBottom = std::min(rectangleDimensions.height + rectanglePosition.top, fbDimensions.height);

    bool areAllColorsSet = true;

    // For each pixel in the frame buffer where the image will be drawn
    for (uint32_t y = fbTop; y < fbBottom; y++) {
        for (uint32_t x = fbLeft; x < fbRight; x++) {
            const PixelCoordinates rectCoords{ x - rectanglePosition.left, y - rectanglePosition.top };
            const PixelCoordinates fbCoords{ x, y };

            const bool arerectCoordsInInnerFill =
                rectCoords.left < innerStrokeWidth ||
                rectCoords.left >= rectangleDimensions.width - innerStrokeWidth ||
                rectCoords.top < innerStrokeWidth ||
                rectCoords.top >= rectangleDimensions.height - innerStrokeWidth;

            const PixelColor frameBufferColor = frameBuffer.GetColor(fbCoords);
            const PixelColor blendedPixelColor = arerectCoordsInInnerFill
                ? PixelColor::Blend(frameBufferColor, strokeColor)
                : PixelColor::Blend(frameBufferColor, fillColor);

            areAllColorsSet &= frameBuffer.SetColor(blendedPixelColor, fbCoords);
        }
    }

    return areAllColorsSet;
}
