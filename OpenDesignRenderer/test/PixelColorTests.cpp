#include <memory>
#include <gtest/gtest.h>

#include <OpenDesignRenderer/PixelColor.h>


namespace {
constexpr odr::PixelColor COLOR_LIGHT_GREEN{ 0x70, 0xF0, 0x70, 0x80 };
constexpr odr::PixelColor COLOR_LIGHT_RED{ 0xF0, 0x30, 0x30, 0x80 };
constexpr odr::PixelColor COLOR_DARK_RED{ 0xF0, 0x70, 0x70, 0x80 };
constexpr odr::PixelColor COLOR_RED_05{ 0xFF, 0, 0, 0x80 };
constexpr odr::PixelColor COLOR_BLUE_10{ 0, 0, 0xFF, 0xFF };
}

//! PixelColor class tests.
class PixelColorTests : public ::testing::Test {
};


TEST_F(PixelColorTests, AlphaBlend) {
    const auto lightRedOverTransparent = odr::PixelColor::Blend(odr::COLOR_TRANSPARENT, COLOR_LIGHT_RED);
    ASSERT_EQ(lightRedOverTransparent, COLOR_LIGHT_RED);

    const auto transparentOverLighRed = odr::PixelColor::Blend(COLOR_LIGHT_RED, odr::COLOR_TRANSPARENT);
    ASSERT_EQ(transparentOverLighRed, COLOR_LIGHT_RED);

    const auto lightRedOverLightGreen = odr::PixelColor::Blend(COLOR_LIGHT_GREEN, COLOR_LIGHT_RED);
    const odr::PixelColor lrolgColor{ 0xC5, 0x70, 0x45, 0xC0 };
    ASSERT_EQ(lightRedOverLightGreen, lrolgColor);

    const auto lightGreenOverLightRed = odr::PixelColor::Blend(COLOR_LIGHT_RED, COLOR_LIGHT_GREEN);
    const odr::PixelColor lgolrColor{ 0x9B, 0xB0, 0x5B, 0xC0 };
    ASSERT_EQ(lightGreenOverLightRed, lgolrColor);

    const auto lightRedOverDarkRed = odr::PixelColor::Blend(COLOR_DARK_RED, COLOR_LIGHT_RED);
    const odr::PixelColor lrodrColor{ 0xF0, 0x45, 0x45, 0xC0 };
    ASSERT_EQ(lightRedOverDarkRed, lrodrColor);

    const auto darkRedOverLightRed = odr::PixelColor::Blend(COLOR_LIGHT_RED, COLOR_DARK_RED);
    const odr::PixelColor drolrColor{ 0xF0, 0x5B, 0x5B, 0xC0 };
    ASSERT_EQ(darkRedOverLightRed, drolrColor);

    const auto red05OverBlue10 = odr::PixelColor::Blend(COLOR_BLUE_10, COLOR_RED_05);
    const odr::PixelColor r05ob10{ 0x80, 0, 0x7F, 0xFF };
    ASSERT_EQ(red05OverBlue10, r05ob10);

    const auto blue10Overred05 = odr::PixelColor::Blend(COLOR_RED_05, COLOR_BLUE_10);
    ASSERT_EQ(blue10Overred05, COLOR_BLUE_10);
}
