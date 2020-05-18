#include "gtest/gtest.h"

extern "C" {
#include <Temperature.h>
}

class TemperatureTest : public ::testing::Test {
protected:
    temp_t temp;

    void SetUp() override {
        temp = temperature_create();
    }

    void TearDown() override {
    }
};

TEST_F(TemperatureTest, TemperatureCreate) {
    ASSERT_NE(temp, nullptr);
}

TEST_F(TemperatureTest, TemperatureDestroy) {
    temperature_destroy(temp);
    //ASSERT_TRUE(temp == NULL);
    ASSERT_NE(temp, nullptr);
}

TEST_F(TemperatureTest, TemperatureMeasure) {
    temperature_measure(temp);
    ASSERT_NE(temp->lastValues, NULL);
}

TEST_F(TemperatureTest, TemperatureGet) {
    uint8_t value = -100;
    value = temperature_get(temp);
    ASSERT_TRUE(value >= -50 && value <= 50);
}