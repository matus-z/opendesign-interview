#include <OpenDesignRenderer/PixelColor.h>

#include <cmath>

namespace {
    float To01(unsigned char value) {
        return static_cast<float>(value) / 255.0;
    }
    unsigned char To255(float value) {
        return static_cast<unsigned char>(std::round(value * 255.0));
    }
}

uint32_t odr::PixelColor::RGBA() const {
    const uint32_t colorUint = (r << 24 | g << 16 | b << 8 | a);
    return colorUint;
}

bool odr::PixelColor::operator==(const PixelColor& other) const {
    return
        r == other.r &&
        g == other.g &&
        b == other.b &&
        a == other.a;
}

/*static*/ odr::PixelColor odr::PixelColor::Interpolate(const PixelColor& color0, const PixelColor& color1, float t) {
    if (t <= 0) {
        return color0;
    } else if (t >= 1) {
        return color1;
    } else {
        const float r0 = static_cast<float>(color0.r);
        const float g0 = static_cast<float>(color0.g);
        const float b0 = static_cast<float>(color0.b);
        const float a0 = static_cast<float>(color0.a);

        const float r1 = static_cast<float>(color1.r);
        const float g1 = static_cast<float>(color1.g);
        const float b1 = static_cast<float>(color1.b);
        const float a1 = static_cast<float>(color1.a);

        const float rF = (1.0 - t) * r0 + t * r1;
        const float gF = (1.0 - t) * g0 + t * g1;
        const float bF = (1.0 - t) * b0 + t * b1;
        const float aF = (1.0 - t) * a0 + t * a1;

        const unsigned char r = static_cast<unsigned char>(std::round(rF));
        const unsigned char g = static_cast<unsigned char>(std::round(gF));
        const unsigned char b = static_cast<unsigned char>(std::round(bF));
        const unsigned char a = static_cast<unsigned char>(std::round(aF));

        return PixelColor{ r, g, b, a };
    }
}

/*static*/ odr::PixelColor odr::PixelColor::Blend(const PixelColor& bgColor, const PixelColor& fgColor) {
    if (fgColor.a <= 0 && bgColor.a <= 0) {
        return PixelColor {
            To255((To01(bgColor.r) + To01(fgColor.r)) / 2.0f),
            To255((To01(bgColor.g) + To01(fgColor.g)) / 2.0f),
            To255((To01(bgColor.b) + To01(fgColor.b)) / 2.0f),
            0
        };
    } if (fgColor.a <= 0u) {
        return bgColor;
    } else if (fgColor.a >= 255u || bgColor.a <= 0u) {
        return fgColor;
    }

    const float bgr01 = To01(bgColor.r);
    const float bgg01 = To01(bgColor.g);
    const float bgb01 = To01(bgColor.b);
    const float fgr01 = To01(fgColor.r);
    const float fgg01 = To01(fgColor.g);
    const float fgb01 = To01(fgColor.b);

    const float bga01 = To01(bgColor.a);
    const float fga01 = To01(fgColor.a);
    const float fga01inv = 1.0 - fga01;

    const float resa01 = fga01 + fga01inv * bga01;
    const float bga01xfga01inv = bga01 * fga01inv;

    return PixelColor {
        To255((fgr01 * fga01 + bgr01 * bga01xfga01inv) / resa01),
        To255((fgg01 * fga01 + bgg01 * bga01xfga01inv) / resa01),
        To255((fgb01 * fga01 + bgb01 * bga01xfga01inv) / resa01),
        To255(resa01)
    };
}
