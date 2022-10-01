#include <memory>
#include <gtest/gtest.h>

#include <OpenDesignRenderer/Image.h>
#include <OpenDesignRenderer/PixelCoordinates.h>
#include <OpenDesignRenderer/PixelColor.h>
#include <OpenDesignRenderer/RenderingEngine.h>


namespace {
constexpr odr::PixelColor COLOR_LIGHT_GREEN{ 0x70, 0xF0, 0x70, 0x80 };
constexpr odr::PixelColor COLOR_DARK_RED{ 0xF0, 0x30, 0x30, 0x80 };
constexpr odr::PixelColor COLOR_TRANSPARENT_BLUE{ 0x00, 0x00, 0xFF, 0x20 };
constexpr odr::PixelColor COLOR_GB_40{ 0x00, 0x40, 0x40, 0xFF };
}

//! RenderingEngine class tests.
class RenderingEngineTests : public ::testing::Test {
protected:
    virtual void SetUp() override {
    }
};

TEST_F(RenderingEngineTests, InitializeTransparent) {
    odr::RenderingEngine engine;

    const odr::ImageDimensions frameBufferDimensions{ 800, 500 };
    const bool isBufferInitialized = engine.InitializeFrameBuffer(frameBufferDimensions);
    ASSERT_TRUE(isBufferInitialized);

    odr::Image renderedImage;
    const bool isRendered = engine.Render(renderedImage);
    ASSERT_TRUE(isRendered);

    ASSERT_EQ(renderedImage.GetDimensions(), frameBufferDimensions);

    for (uint32_t y = 0u; y < frameBufferDimensions.height; y++) {
        for (uint32_t x = 0u; x < frameBufferDimensions.width; x++) {
            const odr::PixelColor pixelColor = renderedImage.GetColor(odr::PixelCoordinates{ x, y });
            ASSERT_EQ(pixelColor, odr::COLOR_TRANSPARENT);
        }
    }
}

TEST_F(RenderingEngineTests, DrawRectangle) {
    odr::RenderingEngine engine;

    const odr::ImageDimensions frameBufferDimensions{ 800, 500 };
    const bool isBufferInitialized = engine.InitializeFrameBuffer(frameBufferDimensions);
    ASSERT_TRUE(isBufferInitialized);

    const odr::PixelCoordinatesUnbounded rectanglePosition{ 50, 50 };
    const odr::ImageDimensions rectangleDimensions{ 200, 100 };
    const bool isRectangleDrawn = engine.DrawRectangle(rectanglePosition, rectangleDimensions, COLOR_LIGHT_GREEN, 0, COLOR_LIGHT_GREEN);
    ASSERT_TRUE(isRectangleDrawn);

    odr::Image renderedImage;
    const bool isRendered = engine.Render(renderedImage);
    ASSERT_TRUE(isRendered);

    const bool isImgSaved = renderedImage.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-rendered-rectangle_800x500.rgba");
    ASSERT_TRUE(isImgSaved);

    for (uint32_t y = 0u; y < frameBufferDimensions.height; y++) {
        for (uint32_t x = 0u; x < frameBufferDimensions.width; x++) {
            const odr::PixelColor pixelColor = renderedImage.GetColor(odr::PixelCoordinates{ x, y });

            if (
                x >= rectanglePosition.left &&
                x < rectanglePosition.left + rectangleDimensions.width &&
                y >= rectanglePosition.top &&
                y < rectanglePosition.top + rectangleDimensions.height) {
                ASSERT_EQ(pixelColor, COLOR_LIGHT_GREEN);
            } else {
                ASSERT_EQ(pixelColor, odr::COLOR_TRANSPARENT);
            }
        }
    }
}

TEST_F(RenderingEngineTests, DrawRectangleInnerStroke) {
    odr::RenderingEngine engine;

    const odr::ImageDimensions frameBufferDimensions{ 640, 480 };
    const bool isBufferInitialized = engine.InitializeFrameBuffer(frameBufferDimensions);
    ASSERT_TRUE(isBufferInitialized);

    const odr::PixelCoordinatesUnbounded rectanglePosition{ 8, 8 };
    const odr::ImageDimensions rectangleDimensions{ 624, 464 };
    const uint32_t strokeWidth = 24;
    const bool isRectangleDrawn = engine.DrawRectangle(rectanglePosition, rectangleDimensions, COLOR_LIGHT_GREEN, strokeWidth, COLOR_DARK_RED);
    ASSERT_TRUE(isRectangleDrawn);

    odr::Image renderedImage;
    const bool isRendered = engine.Render(renderedImage);
    ASSERT_TRUE(isRendered);

    const bool isImgSaved = renderedImage.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-rendered-rectangle-innerstroke_640x480.rgba");
    ASSERT_TRUE(isImgSaved);

    for (uint32_t y = 0u; y < frameBufferDimensions.height; y++) {
        for (uint32_t x = 0u; x < frameBufferDimensions.width; x++) {
            const odr::PixelColor pixelColor = renderedImage.GetColor(odr::PixelCoordinates{ x, y });

            const bool isInRectangle = 
                x >= rectanglePosition.left &&
                x < rectanglePosition.left + rectangleDimensions.width &&
                y >= rectanglePosition.top &&
                y < rectanglePosition.top + rectangleDimensions.height;
            const bool isInInnerRectangle =
                x >= rectanglePosition.left + strokeWidth &&
                x + strokeWidth < rectanglePosition.left + rectangleDimensions.width &&
                y >= rectanglePosition.top + strokeWidth &&
                y + strokeWidth < rectanglePosition.top + rectangleDimensions.height;

            if (isInInnerRectangle) {
                ASSERT_EQ(pixelColor, COLOR_LIGHT_GREEN);
            } else if (isInRectangle) {
                ASSERT_EQ(pixelColor, COLOR_DARK_RED);
            } else {
                ASSERT_EQ(pixelColor, odr::COLOR_TRANSPARENT);
            }
        }
    }
}

TEST_F(RenderingEngineTests, DrawRectangleBlend) {
    odr::RenderingEngine engine;

    const odr::ImageDimensions frameBufferDimensions{ 800, 500 };
    const bool isBufferInitialized = engine.InitializeFrameBuffer(frameBufferDimensions);
    ASSERT_TRUE(isBufferInitialized);

    const odr::PixelCoordinatesUnbounded greenRectanglePosition{ 50, 50 };
    const odr::ImageDimensions greenRectangleDimensions{ 200, 100 };
    const bool isGreenRectangleDrawn = engine.DrawRectangle(greenRectanglePosition, greenRectangleDimensions, COLOR_LIGHT_GREEN, 0, COLOR_LIGHT_GREEN);
    ASSERT_TRUE(isGreenRectangleDrawn);

    const odr::PixelCoordinatesUnbounded redRectanglePosition{ 200, 100 };
    const odr::ImageDimensions redRectangleDimensions{ 100, 20 };
    const bool isRedRectangleDrawn = engine.DrawRectangle(redRectanglePosition, redRectangleDimensions, COLOR_DARK_RED, 0, COLOR_DARK_RED);
    ASSERT_TRUE(isRedRectangleDrawn);

    odr::Image renderedImage;
    const bool isRendered = engine.Render(renderedImage);
    ASSERT_TRUE(isRendered);

    const auto colorBlended = odr::PixelColor::Blend(COLOR_LIGHT_GREEN, COLOR_DARK_RED);

    for (uint32_t y = 0u; y < frameBufferDimensions.height; y++) {
        for (uint32_t x = 0u; x < frameBufferDimensions.width; x++) {
            const odr::PixelColor pixelColor = renderedImage.GetColor(odr::PixelCoordinates{ x, y });

            const bool isInGreenImage =
                x >= greenRectanglePosition.left &&
                x < greenRectanglePosition.left + greenRectangleDimensions.width &&
                y >= greenRectanglePosition.top &&
                y < greenRectanglePosition.top + greenRectangleDimensions.height;
            const bool isInRedImage =
                x >= redRectanglePosition.left &&
                x < redRectanglePosition.left + redRectangleDimensions.width &&
                y >= redRectanglePosition.top &&
                y < redRectanglePosition.top + redRectangleDimensions.height;

            if (isInGreenImage && isInRedImage) {
                ASSERT_EQ(pixelColor, colorBlended);
            } else if (isInGreenImage) {
                ASSERT_EQ(pixelColor, COLOR_LIGHT_GREEN);
            } else if (isInRedImage) {
                ASSERT_EQ(pixelColor, COLOR_DARK_RED);
            } else {
                ASSERT_EQ(pixelColor, odr::COLOR_TRANSPARENT);
            }
        }
    }

    const bool isImgSaved = renderedImage.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-rendered-rectangle_blend_800x500.rgba");
    ASSERT_TRUE(isImgSaved);
}

TEST_F(RenderingEngineTests, DrawImage) {
    odr::RenderingEngine engine;

    odr::Image imgA;
    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);

    const odr::ImageDimensions frameBufferDimensions{ 800, 500 };
    const bool isBufferInitialized = engine.InitializeFrameBuffer(frameBufferDimensions);
    ASSERT_TRUE(isBufferInitialized);

    const odr::PixelCoordinatesUnbounded imgPosition{ 20, 20 };
    const odr::ImageDimensions imgDimensions{ 200, 100 };
    const bool isImgDrawn = engine.Draw(imgA, imgPosition, imgDimensions);
    ASSERT_TRUE(isImgDrawn);

    odr::Image renderedImage;
    const bool isRendered = engine.Render(renderedImage);
    ASSERT_TRUE(isRendered);

    const bool isImgSaved = renderedImage.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-rendered-image_800x500.rgba");
    ASSERT_TRUE(isImgSaved);
}

TEST_F(RenderingEngineTests, DrawComposite) {
    odr::RenderingEngine engine;

    odr::Image imgA;
    const bool isImgALoaded = imgA.Load(std::string(TESTING_IMAGES_DIR) + "image-A.rgba");
    ASSERT_TRUE(isImgALoaded);

    odr::Image imgB;
    const bool isImgBLoaded = imgB.Load(std::string(TESTING_IMAGES_DIR) + "image-B.rgba");
    ASSERT_TRUE(isImgBLoaded);

    odr::Image imgC;
    const bool isImgCLoaded = imgC.Load(std::string(TESTING_IMAGES_DIR) + "image-C.rgba");
    ASSERT_TRUE(isImgCLoaded);

    const odr::ImageDimensions frameBufferDimensions{ 640, 480 };
    const bool isBufferInitialized = engine.InitializeFrameBuffer(frameBufferDimensions);
    ASSERT_TRUE(isBufferInitialized);

    {
        const odr::PixelCoordinatesUnbounded imgPosition{ -40, 60 };
        const odr::ImageDimensions imgDimensions{ 720, 360 };
        const bool isImgDrawn = engine.Draw(imgA, imgPosition, imgDimensions);
        ASSERT_TRUE(isImgDrawn);
    }
    {
        const odr::PixelCoordinatesUnbounded rectanglePosition{ 8, 8 };
        const odr::ImageDimensions rectangleDimensions{ 624, 464 };
        const uint32_t strokeWidth = 24;
        const bool isRectangleDrawn = engine.DrawRectangle(rectanglePosition, rectangleDimensions, COLOR_TRANSPARENT_BLUE, strokeWidth, COLOR_GB_40);
        ASSERT_TRUE(isRectangleDrawn);
    }
    {
        const odr::PixelCoordinatesUnbounded imgPosition{ 0, 0 };
        const odr::ImageDimensions imgDimensions{ 640, 480 };
        const bool isImgDrawn = engine.Draw(imgB, imgPosition, imgDimensions);
        ASSERT_TRUE(isImgDrawn);
    }
    {
        const odr::PixelCoordinatesUnbounded imgPosition{ 0, 0 };
        const odr::ImageDimensions imgDimensions{ 256, 256 };
        const bool isImgDrawn = engine.Draw(imgC, imgPosition, imgDimensions);
        ASSERT_TRUE(isImgDrawn);
    }

    odr::Image renderedImage;
    const bool isRendered = engine.Render(renderedImage);
    ASSERT_TRUE(isRendered);

    const bool isImgSaved = renderedImage.Save(std::string(TESTING_IMAGES_DIR) + "tmp_image-rendered-composite_640x480.rgba");
    ASSERT_TRUE(isImgSaved);
}
