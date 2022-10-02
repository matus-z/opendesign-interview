#include <OpenDesignRenderer/PixelColor.h>

#include <cmath>

namespace {
    //! Convert an unsigned char value in the range <0, 255> to <0.0, 1.0>
    inline float To01(unsigned char value) {
        return static_cast<float>(value) / 255.0;
    }
    //! Convert a real number value in <0.0, 1.0> to <0, 255>.
    inline unsigned char To255(float value) {
        if (value <= 0.0f) {
            return 0;
        }
        else if (value >= 1.0f) {
            return 0xFF;
        }
        else {
            return static_cast<unsigned char>(std::round(value * 255.0));
        }
    }
    //! Compute the absolute difference between unsigned chars.
    inline unsigned char UCharAbsoluteDiff(unsigned char charA, unsigned char charB) {
        return charA >= charB
            ? charA - charB
            : charB - charA;
    }
}


/*static*/ odr::PixelColor odr::PixelColor::RGBAlpha(unsigned char r, unsigned char g, unsigned char b, float alphaAsPercentage) {
    return PixelColor {
        r,
        g,
        b,
        To255(alphaAsPercentage / 100.0f)
    };
}

bool odr::PixelColor::operator==(const PixelColor& other) const {
    return
        r == other.r &&
        g == other.g &&
        b == other.b &&
        a == other.a;
}

/*static*/ odr::PixelColor odr::PixelColor::Blend(const PixelColor& bgColor, const PixelColor& fgColor) {
    if (fgColor.a <= 0 && bgColor.a <= 0) {
        return PixelColor{
            To255((To01(bgColor.r) + To01(fgColor.r)) / 2.0f),
            To255((To01(bgColor.g) + To01(fgColor.g)) / 2.0f),
            To255((To01(bgColor.b) + To01(fgColor.b)) / 2.0f),
            0
        };
    } if (fgColor.a <= 0u) {
        return bgColor;
    }
    else if (fgColor.a >= 255u || bgColor.a <= 0u) {
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

    return PixelColor{
        To255((fgr01 * fga01 + bgr01 * bga01xfga01inv) / resa01),
        To255((fgg01 * fga01 + bgg01 * bga01xfga01inv) / resa01),
        To255((fgb01 * fga01 + bgb01 * bga01xfga01inv) / resa01),
        To255(resa01)
    };
}

/*static*/ odr::PixelColor odr::PixelColor::AbsoluteDiff(const PixelColor& colorA, const PixelColor& colorB) {
    return PixelColor {
        UCharAbsoluteDiff(colorA.r, colorB.r),
        UCharAbsoluteDiff(colorA.g, colorB.g),
        UCharAbsoluteDiff(colorA.b, colorB.b),
        UCharAbsoluteDiff(colorA.a, colorB.a)
    };
}
