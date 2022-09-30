#pragma once

#include <stdint.h>

namespace odr {
//! Pixel color in RGBA format.
struct PixelColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    uint32_t RGBA() const;

    //! Detect if other color is identical to this one.
    bool operator==(const PixelColor& other) const;

    //! Linearly interpolate two colors.
    static PixelColor Interpolate(const PixelColor& color0, const PixelColor& color1, float t);
};
//! PixelColor presets
constexpr PixelColor COLOR_TRANSPARENT{ 0, 0, 0, 0 };
}
