#include "gtest/gtest.h"

extern "C" {
#include <Humidity.h>
}



class HumidityTest : public ::testing::Test {
protected:
    hum_t hum = NULL;
    void SetUp() override {
         hum = humidity_create();
    }

    void TearDown() override {
        humidity_destroy(&hum);
    }
};

TEST_F(HumidityTest, HumidityCreate) {    
    ASSERT_NE(hum, nullptr);
}

TEST_F(HumidityTest, HumidityDestroy) {
    humidity_destroy(&hum);
    ASSERT_EQ(hum, nullptr);
}

TEST_F(HumidityTest, HumidityMeasure) {
    humidity_measure(hum);
    ASSERT_NE(hum->lastValues, NULL);
}

TEST_F(HumidityTest, HumidityGet) {
    int value = -100;
    value = humidity_get(hum);
    ASSERT_NE(value, -100);
}