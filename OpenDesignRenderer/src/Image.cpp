#include <OpenDesignRenderer/Image.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>

#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>


// Source: https://github.com/bzotto/rgba_bitmap/blob/main/rgba_bitmap.c
// TODO: Refactor.
namespace {

#define RGBA_BITMAP_MAGIC_NUMBER            0x52474241   /* 'RGBA' */

#define READ_BIG_ENDIAN_UINT32(ptr)         (*ptr << 24 | *(ptr+1) << 16 | *(ptr+2)<<8 | *(ptr+3))
#define WRITE_BIG_ENDIAN_UINT32(ptr, u32)   *ptr = (u32 >> 24) & 0xFF; \
                                            *(ptr+1) = (u32 >> 16) & 0xFF; \
                                            *(ptr+2) = (u32 >> 8) & 0xFF; \
                                            *(ptr+3) = u32 & 0xFF;

unsigned char * rgba_encode_bitmap_to_file_data(
    const unsigned char * input_buffer,
    uint32_t width,
    uint32_t height)
{
    if (!input_buffer || width == 0 || height == 0) {
        return nullptr;
    }

    const uint32_t imageDataSize = width * height * 4;
    const uint32_t output_size = 12 + imageDataSize;

    unsigned char * rgba_file = (unsigned char*)malloc(output_size);
    if (!rgba_file) {
        return nullptr;
    }

    WRITE_BIG_ENDIAN_UINT32(&rgba_file[0], RGBA_BITMAP_MAGIC_NUMBER);
    WRITE_BIG_ENDIAN_UINT32(&rgba_file[4], width);
    WRITE_BIG_ENDIAN_UINT32(&rgba_file[8], height);

    memcpy(&rgba_file[12], input_buffer, imageDataSize);

    return rgba_file;
}

unsigned char * rgba_decode_file_data_to_bitmap(
    const unsigned char * file_data,
    uint32_t file_data_length,
    uint32_t * p_width,
    uint32_t * p_height)
{
    if (!file_data || !p_width || !p_height) {
        return nullptr;
    }

    if (file_data_length < 12) {
        return nullptr;
    }

    const uint32_t magic = READ_BIG_ENDIAN_UINT32(&file_data[0]);
    if (magic != RGBA_BITMAP_MAGIC_NUMBER) {
        return nullptr;
    }

    const uint32_t width = READ_BIG_ENDIAN_UINT32(&file_data[4]);
    const uint32_t height = READ_BIG_ENDIAN_UINT32(&file_data[8]);

    const uint32_t imageDataSize = width * height * 4;

    if (file_data_length < (12 + imageDataSize)) {
        return nullptr;
    }

    unsigned char * bitmap = (unsigned char*)malloc(imageDataSize);
    if (!bitmap) {
        return nullptr;
    }

    memcpy(bitmap, &file_data[12], imageDataSize);

    *p_width = width;
    *p_height = height;

    return bitmap;
}
}


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

    imageBuffer = rgba_decode_file_data_to_bitmap(usstr, str.size(), &dimensions.width, &dimensions.height);

    return
        dimensions.width > 0 &&
        dimensions.height > 0 &&
        imageBuffer != nullptr;
}

bool odr::Image::Save(const std::string& filePath) const {
    if (!IsInitialized()) {
        return false;
    }

    const unsigned char* encodedData = rgba_encode_bitmap_to_file_data(imageBuffer, dimensions.width, dimensions.height);
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
