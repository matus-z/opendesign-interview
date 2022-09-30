#include <memory>
#include <gtest/gtest.h>

#include <OpenDesignRenderer/PixelColor.h>


namespace {
constexpr odr::PixelColor COLOR_LIGHT_GREEN{ 0x70, 0xF0, 0x70, 0x80 };
constexpr odr::PixelColor COLOR_DARK_GREEN{ 0x30, 0xF0, 0x30, 0x80 };
}

//! PixelColor class tests.
class PixelColorTests : public ::testing::Test {
};


TEST_F(PixelColorTests, Interpolate) {
    const auto green_15 = odr::PixelColor::Interpolate(COLOR_LIGHT_GREEN, COLOR_DARK_GREEN, 1.5f);
    const auto green_m04 = odr::PixelColor::Interpolate(COLOR_LIGHT_GREEN, COLOR_DARK_GREEN, -0.4f);

    ASSERT_EQ(green_15, COLOR_DARK_GREEN);
    ASSERT_EQ(green_m04, COLOR_LIGHT_GREEN);

    {
        const auto green_07 = odr::PixelColor::Interpolate(COLOR_LIGHT_GREEN, COLOR_DARK_GREEN, 0.7f);

        ASSERT_EQ(green_07.r, 0x43);
        ASSERT_EQ(green_07.g, 0xF0);
        ASSERT_EQ(green_07.b, 0x43);
        ASSERT_EQ(green_07.a, 0x80);
    }

    {
        const auto green_lt_05 = odr::PixelColor::Interpolate(odr::COLOR_TRANSPARENT, COLOR_LIGHT_GREEN, 0.5f);

        ASSERT_EQ(green_lt_05.r, 0x38);
        ASSERT_EQ(green_lt_05.g, 0x78);
        ASSERT_EQ(green_lt_05.b, 0x38);
        ASSERT_EQ(green_lt_05.a, 0x40);
    }
}

TEST_F(PixelColorTests, AlphaBlend) {
}
