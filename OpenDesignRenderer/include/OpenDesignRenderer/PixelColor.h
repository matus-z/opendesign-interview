#pragma once

#include <stdint.h>

namespace odr {
//! Pixel color in RGBA format.
struct PixelColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    //! Initialize PixelColor as RGB values with alpha as a percentage (a value between <0,100>).
    static PixelColor RGBAlpha(unsigned char r, unsigned char g, unsigned char b, float alphaAsPercentage);

    //! Detect if other color is identical to this one.
    bool operator==(const PixelColor& other) const;

    //! Blend color over background.
    static PixelColor Blend(const PixelColor& bgColor, const PixelColor& fgColor);
    //! Compute the absolute difference between two pixels.
    static PixelColor AbsoluteDiff(const PixelColor& colorA, const PixelColor& colorB);
};
//! PixelColor presets
constexpr PixelColor COLOR_TRANSPARENT{ 0, 0, 0, 0 };
}
