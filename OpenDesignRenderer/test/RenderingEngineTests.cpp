#include <memory>
#include <gtest/gtest.h>

#include <OpenDesignRenderer/Image.h>
#include <OpenDesignRenderer/RenderingEngine.h>


//! RenderingEngine class tests.
class RenderingEngineTests : public ::testing::Test {
protected:
    virtual void SetUp() override {
    }
};

TEST_F(RenderingEngineTests, RenderingEngineTests_EQ) {
    odr::Image img;
    odr::RenderingEngine engine;

    EXPECT_EQ(10, 10);
}
