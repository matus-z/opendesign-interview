#include <OpenDesignRenderer/Image.h>

#include <fstream>
#include <sstream>
#include <cmath>

#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>

#include "RgbaBitmap.h"


odr::Image::Image() :
    dimensions(IMAGE_DIMENSIONS_EMPTY),
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
    dimensions = IMAGE_DIMENSIONS_EMPTY;

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

    for (uint32_t top = 0; top < dimensions.height; top++) {
        for (uint32_t left = 0; left < dimensions.width; left++) {
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

    memcpy(imageBuffer, otherImage.imageBuffer, dimensions.DataSize());

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
        return COLOR_TRANSPARENT;
    }

    const uint32_t imageBufferPos = (coords.left + coords.top * dimensions.width) * 4;

    const unsigned char r = imageBuffer[imageBufferPos + 0];
    const unsigned char g = imageBuffer[imageBufferPos + 1];
    const unsigned char b = imageBuffer[imageBufferPos + 2];
    const unsigned char a = imageBuffer[imageBufferPos + 3];

    return PixelColor{ r, g, b, a };
}

bool odr::Image::SetColor(const PixelColor& color, const PixelCoordinates& coords) {
    if (coords.left >= dimensions.width || coords.top >= dimensions.height) {
        return false;
    }

    const uint32_t imageBufferPos = (coords.left + coords.top * dimensions.width) * 4;

    imageBuffer[imageBufferPos + 0] = color.r;
    imageBuffer[imageBufferPos + 1] = color.g;
    imageBuffer[imageBufferPos + 2] = color.b;
    imageBuffer[imageBufferPos + 3] = color.a;

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

odr::Image odr::Image::Scaled(const ImageDimensions& newDimensions) const {
    Image scaledImage;

    if (dimensions == newDimensions) {
        scaledImage.Initialize(*this);
        return scaledImage;
    }

    const bool isInitialized = scaledImage.Initialize(newDimensions, COLOR_TRANSPARENT);
    if (!isInitialized) {
        return scaledImage;
    }

    const float scalingFactorX = static_cast<float>(newDimensions.width) / static_cast<float>(dimensions.width);
    const float scalingFactorY = static_cast<float>(newDimensions.height) / static_cast<float>(dimensions.height);

    // TODO: Use bilinear interpolation to resample instead of taking the nearest pixel color
    for (uint32_t top = 0; top < newDimensions.height; top++) {
        for (uint32_t left = 0; left < newDimensions.width; left++) {
            const float oldXF = static_cast<float>(left) / scalingFactorX;
            const float oldYF = static_cast<float>(top) / scalingFactorY;

            const uint32_t oldX = static_cast<uint32_t>(std::round(oldXF));
            const uint32_t oldY = static_cast<uint32_t>(std::round(oldYF));

            const PixelCoordinates oldNearestPixelCoordinats{ oldX, oldY };

            const PixelCoordinates newPixelCoordinates{ left, top };
            const PixelColor newPixelColor = GetColor(oldNearestPixelCoordinats);

            const bool isSet = scaledImage.SetColor(newPixelColor, newPixelCoordinates);
            if (!isSet) {
                return scaledImage;
            }
        }
    }

    return scaledImage;
}
