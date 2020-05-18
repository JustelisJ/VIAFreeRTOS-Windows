#include "gtest/gtest.h"

extern "C" {
#include <SensorBundle.h>
#include <Temperature.h>
#include <Humidity.h>
}

class SensorBundleTest : public ::testing::Test {
protected:
    temp_t temp = NULL;
    hum_t hum = NULL;
    bundle_t bundle = NULL;

    void SetUp() override {
        temp = temperature_create();
        hum = humidity_create();
        bundle = SensorBundle_create(temp, hum);
    }

    void TearDown() override {
        temperature_destroy(temp);
        humidity_destroy(hum);
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