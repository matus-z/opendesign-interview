#include <OpenDesignRenderer/RenderingEngine.h>

#include <algorithm>

#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>


bool odr::RenderingEngine::SetFrameBuffer(const ImageDimensions& dimensions) {
    return frameBuffer.Initialize(dimensions, COLOR_TRANSPARENT);
}

bool odr::RenderingEngine::CopyFrameBuffer(Image& image) const {
    return image.Initialize(frameBuffer);
}

bool odr::RenderingEngine::Draw(
    const Image& image,
    const PixelCoordinatesUnbounded& position,
    const ImageDimensions& dimensions) {
    const Image scaledImage = image.Scaled(dimensions);

    const ImageDimensions& frameBufferDimensions = frameBuffer.GetDimensions();

    const uint32_t fbLeft = std::max(position.left, 0);
    const uint32_t fbRight = std::min(dimensions.width + position.left, frameBufferDimensions.width);
    const uint32_t fbTop = std::max(position.top, 0);
    const uint32_t fbBottom = std::min(dimensions.height + position.top, frameBufferDimensions.height);

    // For each pixel in the frame buffer where the image will be drawn
    for (uint32_t y = fbTop; y < fbBottom; y++) {
        for (uint32_t x = fbLeft; x < fbRight; x++) {
            const PixelCoordinates imageCoords{ x - position.left, y - position.top };
            const PixelCoordinates frameBufferCoordinates{ x, y };

            const PixelColor imageColor = scaledImage.GetColor(imageCoords);
            const PixelColor frameBufferColor = frameBuffer.GetColor(frameBufferCoordinates);

            const PixelColor blendedPixelColor = PixelColor::Blend(frameBufferColor, imageColor);
            const bool isColorSet = frameBuffer.SetColor(blendedPixelColor, frameBufferCoordinates);
            if (!isColorSet) {
                return false;
            }
        }
    }

    return true;
}

bool odr::RenderingEngine::DrawRectangle(
    const PixelCoordinatesUnbounded& position,
    const ImageDimensions& dimensions,
    const PixelColor& fillColor,
    uint32_t innerStrokeWidth,
    const PixelColor& strokeColor) {
    const ImageDimensions& frameBufferDimensions = frameBuffer.GetDimensions();

    const uint32_t fbLeft = std::max(position.left, 0);
    const uint32_t fbRight = std::min(dimensions.width + position.left, frameBufferDimensions.width);
    const uint32_t fbTop = std::max(position.top, 0);
    const uint32_t fbBottom = std::min(dimensions.height + position.top, frameBufferDimensions.height);

    // For each pixel in the frame buffer where the image will be drawn
    for (uint32_t y = fbTop; y < fbBottom; y++) {
        for (uint32_t x = fbLeft; x < fbRight; x++) {
            const PixelCoordinates rectangleCoords{ x - position.left, y - position.top };
            const PixelCoordinates frameBufferCoordinates{ x, y };

            const bool areRectangleCoordsInInnerFill =
                rectangleCoords.left < innerStrokeWidth ||
                rectangleCoords.left >= dimensions.width - innerStrokeWidth ||
                rectangleCoords.top < innerStrokeWidth ||
                rectangleCoords.top >= dimensions.height - innerStrokeWidth;

            const PixelColor rectangleColor = areRectangleCoordsInInnerFill
                ? strokeColor
                : fillColor;
            const PixelColor frameBufferColor = frameBuffer.GetColor(frameBufferCoordinates);

            const PixelColor blendedPixelColor = PixelColor::Blend(frameBufferColor, rectangleColor);
            const bool isColorSet = frameBuffer.SetColor(blendedPixelColor, frameBufferCoordinates);
            if (!isColorSet) {
                return false;
            }
        }
    }

    return true;
}
