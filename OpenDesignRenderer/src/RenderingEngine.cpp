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

    const ImageDimensions& frameBufferDimensions = frameBuffer.GetDimensions();

    const uint32_t fbLeft = std::max(imagePosition.left, 0);
    const uint32_t fbRight = std::min(imageDimensions.width + imagePosition.left, frameBufferDimensions.width);
    const uint32_t fbTop = std::max(imagePosition.top, 0);
    const uint32_t fbBottom = std::min(imageDimensions.height + imagePosition.top, frameBufferDimensions.height);

    // For each pixel in the frame buffer where the image will be drawn
    for (uint32_t y = fbTop; y < fbBottom; y++) {
        for (uint32_t x = fbLeft; x < fbRight; x++) {
            const PixelCoordinates imageCoords{ x - imagePosition.left, y - imagePosition.top };
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
    const PixelCoordinatesUnbounded& rectanglePosition,
    const ImageDimensions& rectangleDimensions,
    const PixelColor& fillColor,
    uint32_t innerStrokeWidth,
    const PixelColor& strokeColor) {
    const ImageDimensions& frameBufferDimensions = frameBuffer.GetDimensions();

    const uint32_t fbLeft = std::max(rectanglePosition.left, 0);
    const uint32_t fbRight = std::min(rectangleDimensions.width + rectanglePosition.left, frameBufferDimensions.width);
    const uint32_t fbTop = std::max(rectanglePosition.top, 0);
    const uint32_t fbBottom = std::min(rectangleDimensions.height + rectanglePosition.top, frameBufferDimensions.height);

    // For each pixel in the frame buffer where the image will be drawn
    for (uint32_t y = fbTop; y < fbBottom; y++) {
        for (uint32_t x = fbLeft; x < fbRight; x++) {
            const PixelCoordinates rectangleCoords{ x - rectanglePosition.left, y - rectanglePosition.top };
            const PixelCoordinates frameBufferCoordinates{ x, y };

            const bool areRectangleCoordsInInnerFill =
                rectangleCoords.left < innerStrokeWidth ||
                rectangleCoords.left >= rectangleDimensions.width - innerStrokeWidth ||
                rectangleCoords.top < innerStrokeWidth ||
                rectangleCoords.top >= rectangleDimensions.height - innerStrokeWidth;

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
