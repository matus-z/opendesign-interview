#pragma once

#include <OpenDesignRenderer/Image.h>

namespace odr {
//! Simple 2D rendering engine.
class RenderingEngine {
public:
    explicit RenderingEngine();

    // void SetFrameBuffer(uint32_t width, uint32_t height);
    // void Draw(const Image& image, uint32_t left, uint32_t top, uint32_t width, uint32_t height);
    // void DrawRectangle(uint32_t left, uint32_t top, uint32_t width, uint32_t height, char fillFolor, float alpha, uint32_t innerStrokeWidth, char innerStrokeColor, float innerStrokeAlpha);

    // void CopyFrameBuffer(Image& image);

private:
    Image frameBuffer;
};
}
