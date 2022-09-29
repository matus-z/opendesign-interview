#include <OpenDesignRenderer/PixelColor.h>


unsigned char odr::PixelColor::R() const {
    unsigned char *p = (unsigned char*)&colorRGBA;
    return p[3];
}
unsigned char odr::PixelColor::G() const {
    unsigned char *p = (unsigned char*)&colorRGBA;
    return p[2];
}
unsigned char odr::PixelColor::B() const {
    unsigned char *p = (unsigned char*)&colorRGBA;
    return p[1];
}
unsigned char odr::PixelColor::A() const {
    unsigned char *p = (unsigned char*)&colorRGBA;
    return p[0];
}

bool odr::PixelColor::operator==(const PixelColor& other) const {
    return colorRGBA == other.colorRGBA;
}
