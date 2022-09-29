#pragma once

#include <stdint.h>

namespace odr {
//! Pixel color in RGBA format.
struct PixelColor {
    uint32_t colorRGBA;

    unsigned char R() const;
    unsigned char G() const;
    unsigned char B() const;
    unsigned char A() const;

    //! Detect if other color is identical to this one.
    bool operator==(const PixelColor& other) const;
};
//! PixelColor presets
constexpr PixelColor TRANSPARENT_COLOR{ 0 };
constexpr PixelColor COLOR_LIGHT_GREEN{ 1894805632 };
}
