#include <OpenDesignRenderer/PixelColor.h>

#include <cmath>


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
