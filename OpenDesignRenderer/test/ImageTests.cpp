#include <memory>
#include <gtest/gtest.h>

#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>
#include <OpenDesignRenderer/Image.h>


namespace {
constexpr odr::PixelColor COLOR_LIGHT_GREEN{ 0x70, 0xF0, 0x70, 0x80 };
constexpr odr::PixelColor COLOR_DARK_GREEN{ 0x30, 0xF0, 0x30, 0x80 };
}

//! Image class tests.
class ImageTests : public ::testing::Test {
protected:
    virtual void SetUp() override {
    }
};

TEST_F(ImageTests, LoadAndSaveConsistency) {
    odr::Image imgA;
    odr::Image imgACopy;

    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);
    const bool isImgASaved = imgA.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-A-copy.rgba");
    ASSERT_TRUE(isImgASaved);

    const bool isImgACopyLoaded = imgACopy.Load(std::string(TESTING_IMAGES_DIR) + "tmp_image-A-copy.rgba");
    ASSERT_TRUE(isImgACopyLoaded);

    ASSERT_EQ(imgA, imgACopy);
}

TEST_F(ImageTests, GetColor) {
    odr::Image imgA;

    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);

    // [0,0]
    {
        const auto pixelColor = imgA.GetColor({0, 0});

        ASSERT_EQ(pixelColor.r, 0xFF);
        ASSERT_EQ(pixelColor.g, 0xFF);
        ASSERT_EQ(pixelColor.b, 0xDA);
        ASSERT_EQ(pixelColor.a, 0x00);
    }

    // [1,0]
    {
        const auto pixelColor = imgA.GetColor({1, 0});

        ASSERT_EQ(pixelColor.r, 0xFF);
        ASSERT_EQ(pixelColor.g, 0xFF);
        ASSERT_EQ(pixelColor.b, 0xF4);
        ASSERT_EQ(pixelColor.a, 0x00);
    }

    // [215, 170]
    {
        const auto pixelColor = imgA.GetColor({215, 170});

        ASSERT_EQ(pixelColor.r, 0x11);
        ASSERT_EQ(pixelColor.g, 0x01);
        ASSERT_EQ(pixelColor.b, 0x0B);
        ASSERT_EQ(pixelColor.a, 0xFF);
    }

    // [480, 150]
    {
        const auto pixelColor = imgA.GetColor({480, 150});

        ASSERT_EQ(pixelColor.r, 0xE7);
        ASSERT_EQ(pixelColor.g, 0xB4);
        ASSERT_EQ(pixelColor.b, 0x99);
        ASSERT_EQ(pixelColor.a, 0xFF);
    }
}

TEST_F(ImageTests, SetColor) {
    odr::Image imgA;

    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);

    // [215, 170]
    {
        const odr::PixelCoordinates PixelCoordinates {215, 170};

        const bool isColorWritten = imgA.SetColor(COLOR_DARK_GREEN, PixelCoordinates);
        ASSERT_TRUE(isColorWritten);

        const auto pixelColor = imgA.GetColor(PixelCoordinates);

        ASSERT_EQ(pixelColor.r, COLOR_DARK_GREEN.r);
        ASSERT_EQ(pixelColor.g, COLOR_DARK_GREEN.g);
        ASSERT_EQ(pixelColor.b, COLOR_DARK_GREEN.b);
        ASSERT_EQ(pixelColor.a, COLOR_DARK_GREEN.a);
    }
}

TEST_F(ImageTests, InitializeWithColor) {
    odr::Image imgLightGreen;

    const odr::ImageDimensions dimensions{ 60, 40 };

    const bool isInitialized = imgLightGreen.Initialize(dimensions, COLOR_LIGHT_GREEN);
    ASSERT_TRUE(isInitialized);

    ASSERT_EQ(imgLightGreen.GetDimensions(), dimensions);

    for (uint32_t left = 0; left < dimensions.width; left++) {
        for (uint32_t top = 0; top < dimensions.height; top++) {
            const odr::PixelCoordinates PixelCoordinates {left, top};
            const odr::PixelColor pixelColor = imgLightGreen.GetColor(PixelCoordinates);

            ASSERT_EQ(pixelColor, COLOR_LIGHT_GREEN);
        }
    }
}

TEST_F(ImageTests, InitializeAsCopy) {
    odr::Image imgA;

    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);

    odr::Image imgACopy;

    const bool isCopied = imgACopy.Initialize(imgA);
    ASSERT_TRUE(isCopied);

    ASSERT_EQ(imgA, imgACopy);
}

TEST_F(ImageTests, Scaled) {
    odr::Image imgA;

    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);

    // Downscaled to 150x200
    {
        const odr::ImageDimensions scaledDimensions{ 150, 200 };
        const odr::Image imgAScaled = imgA.Scaled(scaledDimensions);

        ASSERT_EQ(imgAScaled.GetDimensions(), scaledDimensions);

        const bool isImgAScaledSaved = imgAScaled.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-A-scaled_150x200.rgba");
        ASSERT_TRUE(isImgAScaledSaved);
    }

    // Upscaled to 1400x500
    {
        const odr::ImageDimensions scaledDimensions{ 1400, 500 };
        const odr::Image imgAScaled = imgA.Scaled(scaledDimensions);

        ASSERT_EQ(imgAScaled.GetDimensions(), scaledDimensions);

        const bool isImgAScaledSaved = imgAScaled.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-A-scaled_1400x500.rgba");
        ASSERT_TRUE(isImgAScaledSaved);
    }

    // Scaled to 300x1000
    {
        const odr::ImageDimensions scaledDimensions{ 300, 1000 };
        const odr::Image imgAScaled = imgA.Scaled(scaledDimensions);

        ASSERT_EQ(imgAScaled.GetDimensions(), scaledDimensions);

        const bool isImgAScaledSaved = imgAScaled.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-A-scaled_300x1000.rgba");
        ASSERT_TRUE(isImgAScaledSaved);
    }
}
