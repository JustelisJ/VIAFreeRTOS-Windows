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
        bundle = sensorBundle_create(temperature_create(), humidity_create());
    }

    void TearDown() override {
        if (bundle != NULL)
        {
            sensorBundle_destroy(&bundle);
        }
    }
};

TEST_F(SensorBundleTest, BundleCreate) {
    EXPECT_NE(bundle, nullptr);
}

TEST_F(SensorBundleTest, BundleDestroy) {
    sensorBundle_destroy(&bundle);
    ASSERT_EQ(bundle, nullptr);
}

TEST_F(SensorBundleTest, BundleGetTemperature) {
    ASSERT_NE(sensorBundle_getTemp(bundle), nullptr);
}

TEST_F(SensorBundleTest, BundleGetHumidity) {
    ASSERT_NE(sensorBundle_getHum(bundle), nullptr);
}