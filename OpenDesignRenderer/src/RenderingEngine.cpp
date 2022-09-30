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
    const PixelCoordinates& position,
    const ImageDimensions& dimensions) {
    const Image scaledImage = image.Scaled(dimensions);

    const ImageDimensions& frameBufferDimensions = frameBuffer.GetDimensions();

    const uint32_t imgLeft = std::max(position.left, 0u);
    const uint32_t imgRight = std::min(dimensions.width + position.left, frameBufferDimensions.width);
    const uint32_t imgTop = std::max(position.top, 0u);
    const uint32_t imgBottom = std::min(dimensions.height + position.top, frameBufferDimensions.height);

    // For each pixel in the image that fits to the frame buffer
    for (uint32_t y = imgTop; y < imgBottom; y++) {
        for (uint32_t x = imgLeft; x < imgRight; x++) {
            const PixelCoordinates imageCoords{ x, y };
            const PixelCoordinates frameBufferCoordinates{ x + position.left, y + position.top };

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
    const PixelCoordinates& position,
    const ImageDimensions& dimensions,
    const PixelColor& fillColor,
    uint32_t innerStrokeWidth,
    const PixelColor& strokeColor) {
    const ImageDimensions& frameBufferDimensions = frameBuffer.GetDimensions();

    const uint32_t rectLeft = std::max(position.left, 0u);
    const uint32_t rectRight = std::min(dimensions.width + position.left, frameBufferDimensions.width);
    const uint32_t rectTop = std::max(position.top, 0u);
    const uint32_t rectBottom = std::min(dimensions.height + position.top, frameBufferDimensions.height);

    // For each pixel in the rectangle that fits to the frame buffer
    for (uint32_t y = rectTop; y < rectBottom; y++) {
        for (uint32_t x = rectLeft; x < rectRight; x++) {
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
