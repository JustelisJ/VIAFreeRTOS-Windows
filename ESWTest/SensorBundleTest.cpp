#include "gtest/gtest.h"

extern "C" {
#include <SensorBundle.h>
#include <Temperature.h>
#include <Humidity.h>
}

class SensorBundleTest : public ::testing::Test {
protected:
    bundle_t bundle = NULL;

    void SetUp() override {
        bundle = SensorBundle_create(temperature_create(), humidity_create());
    }

    void TearDown() override {
        if (bundle != NULL)
        {
            SensorBundle_destroy(bundle);
        }
    }
};

TEST_F(SensorBundleTest, BundleCreate) {
    ASSERT_NE(bundle, nullptr);
}

TEST_F(SensorBundleTest, BundleDestroy) {
    SensorBundle_destroy(bundle);
    ASSERT_EQ(bundle, nullptr);
}

TEST_F(SensorBundleTest, BundleGetTemperature) {
    ASSERT_NE(SensorBundle_getTemp(bundle), nullptr);
}

TEST_F(SensorBundleTest, BundleGetHumidity) {
    ASSERT_NE(SensorBundle_getHum(bundle), nullptr);
}