#pragma once

namespace odr {

//! Pixel color in RGBA format.
struct PixelColor {
    uint32_t colorRGBA;

    unsigned char R() const {
        unsigned char *p = (unsigned char*)&colorRGBA;
        return p[3];
    }
    unsigned char G() const {
        unsigned char *p = (unsigned char*)&colorRGBA;
        return p[2];
    }
    unsigned char B() const {
        unsigned char *p = (unsigned char*)&colorRGBA;
        return p[1];
    }
    unsigned char A() const {
        unsigned char *p = (unsigned char*)&colorRGBA;
        return p[0];
    }

    //! Detect if other color is identical to this one.
    bool operator==(const PixelColor& other) const {
        return colorRGBA == other.colorRGBA;
    }
};
}
