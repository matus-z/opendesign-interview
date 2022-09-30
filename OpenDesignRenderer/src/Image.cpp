#include <OpenDesignRenderer/Image.h>

#include <fstream>
#include <sstream>
#include <cmath>

#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>

#include "RgbaBitmap.h"


#define READ_BIG_ENDIAN_UINT32(ptr)         (*ptr << 24 | *(ptr+1) << 16 | *(ptr+2)<<8 | *(ptr+3))
#define WRITE_BIG_ENDIAN_UINT32(ptr, u32)   *ptr = (u32 >> 24) & 0xFF; \
                                            *(ptr+1) = (u32 >> 16) & 0xFF; \
                                            *(ptr+2) = (u32 >> 8) & 0xFF; \
                                            *(ptr+3) = u32 & 0xFF;


odr::Image::Image() :
    dimensions({0,0}),
    imageBuffer(nullptr) {
}

odr::Image::~Image() {
    if (imageBuffer != nullptr) {
        free(imageBuffer);
    }
}

bool odr::Image::IsInitialized() const {
    return
        dimensions.width > 0 &&
        dimensions.height > 0 &&
        imageBuffer != nullptr;
}

void odr::Image::Clear() {
    dimensions.width = 0;
    dimensions.height = 0;

    if (imageBuffer != nullptr) {
        free(imageBuffer);
        imageBuffer = nullptr;
    }
}

bool odr::Image::Initialize(const ImageDimensions& dimensions_, const PixelColor& color) {
    Clear();

    imageBuffer = (unsigned char*)malloc(dimensions_.DataSize());
    if (imageBuffer == nullptr) {
        Clear();
        return false;
    }

    dimensions = dimensions_;

    for (uint32_t left = 0; left < dimensions.width; left++) {
        for (uint32_t top = 0; top < dimensions.height; top++) {
            const PixelCoordinates PixelCoordinates {left, top};
            const bool isSet = SetColor(color, PixelCoordinates);

            if (!isSet) {
                Clear();
                return false;
            }
        }
    }

    return true;
}

bool odr::Image::Initialize(const Image& otherImage) {
    Clear();

    dimensions = otherImage.GetDimensions();

    imageBuffer = (unsigned char*)malloc(dimensions.DataSize());
    if (imageBuffer == nullptr) {
        Clear();
        return false;
    }

    memcpy(otherImage.imageBuffer, imageBuffer, dimensions.DataSize());

    return true;
}

bool odr::Image::Load(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        return false;
    }

    std::ostringstream ss;
    ss << file.rdbuf();

    const std::string& str = ss.str();
    const char* sscstr = str.c_str();
    const unsigned char* usstr = reinterpret_cast<const unsigned char*>(const_cast<char*>(sscstr));

    imageBuffer = RgbaBitmap::DecodeFromFileData(usstr, str.size(), &dimensions.width, &dimensions.height);

    return
        dimensions.width > 0 &&
        dimensions.height > 0 &&
        imageBuffer != nullptr;
}

bool odr::Image::Save(const std::string& filePath) const {
    if (!IsInitialized()) {
        return false;
    }

    const unsigned char* encodedData = RgbaBitmap::EndcodeToFileData(imageBuffer, dimensions.width, dimensions.height);
    if (encodedData == nullptr) {
        return false;
    }

    std::ofstream file(filePath, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    const uint32_t outputSize = 12 + dimensions.DataSize();
    const char* encd = reinterpret_cast<char*>(const_cast<unsigned char*>(encodedData));
    file.write(encd, outputSize);

    return true;
}

const odr::ImageDimensions& odr::Image::GetDimensions() const {
    return dimensions;
}

odr::PixelColor odr::Image::GetColor(const PixelCoordinates& coords) const {
    if (coords.left >= dimensions.width || coords.top >= dimensions.height) {
        return PixelColor{ 0 };
    }

    return PixelColor{ static_cast<uint32_t>(READ_BIG_ENDIAN_UINT32(&imageBuffer[(coords.left + coords.top * dimensions.width) * 4])) };
}

bool odr::Image::SetColor(const PixelColor& color, const PixelCoordinates& coords) {
    if (coords.left >= dimensions.width || coords.top >= dimensions.height) {
        return false;
    }

    WRITE_BIG_ENDIAN_UINT32(&imageBuffer[(coords.left + coords.top * dimensions.width) * 4], color.colorRGBA);
    return true;
}

bool odr::Image::operator==(const Image& other) const {
    if (IsInitialized() != other.IsInitialized()) {
        return false;
    }

    if (dimensions != other.GetDimensions()) {
        return false;
    }

    if (memcmp(imageBuffer, other.imageBuffer, dimensions.DataSize()) != 0) {
        return false;
    }

    return true;
}
