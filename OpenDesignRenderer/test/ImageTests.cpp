#include <memory>
#include <gtest/gtest.h>

#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>
#include <OpenDesignRenderer/Image.h>


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

        const unsigned char r = pixelColor.R();
        const unsigned char g = pixelColor.G();
        const unsigned char b = pixelColor.B();
        const unsigned char a = pixelColor.A();

        ASSERT_EQ(r, 0xFF);
        ASSERT_EQ(g, 0xFF);
        ASSERT_EQ(b, 0xDA);
        ASSERT_EQ(a, 0x00);
    }

    // [1,0]
    {
        const auto pixelColor = imgA.GetColor({1, 0});

        const unsigned char r = pixelColor.R();
        const unsigned char g = pixelColor.G();
        const unsigned char b = pixelColor.B();
        const unsigned char a = pixelColor.A();

        ASSERT_EQ(r, 0xFF);
        ASSERT_EQ(g, 0xFF);
        ASSERT_EQ(b, 0xF4);
        ASSERT_EQ(a, 0x00);
    }

    // [215, 170]
    {
        const auto pixelColor = imgA.GetColor({215, 170});

        const unsigned char r = pixelColor.R();
        const unsigned char g = pixelColor.G();
        const unsigned char b = pixelColor.B();
        const unsigned char a = pixelColor.A();

        ASSERT_EQ(r, 0x11);
        ASSERT_EQ(g, 0x01);
        ASSERT_EQ(b, 0x0B);
        ASSERT_EQ(a, 0xFF);
    }

    // [480, 150]
    {
        const auto pixelColor = imgA.GetColor({480, 150});

        const unsigned char r = pixelColor.R();
        const unsigned char g = pixelColor.G();
        const unsigned char b = pixelColor.B();
        const unsigned char a = pixelColor.A();

        ASSERT_EQ(r, 0xE7);
        ASSERT_EQ(g, 0xB4);
        ASSERT_EQ(b, 0x99);
        ASSERT_EQ(a, 0xFF);
    }
}

TEST_F(ImageTests, SetColor) {
    odr::Image imgA;

    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);

    // [215, 170]
    {
        const odr::PixelCoordinates PixelCoordinates {215, 170};

        const bool isColorWritten = imgA.SetColor(odr::PixelColor{552607872}, PixelCoordinates);
        ASSERT_TRUE(isColorWritten);

        const auto pixelColor = imgA.GetColor(PixelCoordinates);

        const unsigned char r = pixelColor.R();
        const unsigned char g = pixelColor.G();
        const unsigned char b = pixelColor.B();
        const unsigned char a = pixelColor.A();

        ASSERT_EQ(r, 0x20);
        ASSERT_EQ(g, 0xF0);
        ASSERT_EQ(b, 0x20);
        ASSERT_EQ(a, 0x80);
    }
}

TEST_F(ImageTests, InitializeWithColor) {
    odr::Image imgLightGreen;

    const odr::ImageDimensions dimensions{ 60, 40 };

    const bool isInitialized = imgLightGreen.Initialize(dimensions, odr::COLOR_LIGHT_GREEN);
    ASSERT_TRUE(isInitialized);

    ASSERT_EQ(imgLightGreen.GetDimensions(), dimensions);

    for (uint32_t left = 0; left < dimensions.width; left++) {
        for (uint32_t top = 0; top < dimensions.height; top++) {
            const odr::PixelCoordinates PixelCoordinates {left, top};
            const odr::PixelColor pixelColor = imgLightGreen.GetColor(PixelCoordinates);

            ASSERT_EQ(pixelColor, odr::COLOR_LIGHT_GREEN);
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
