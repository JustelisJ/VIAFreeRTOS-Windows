#include "gtest/gtest.h"

extern "C" {
#include <SensorPacket.h>
}

class SensorDataTest : public ::testing::Test {
protected:
    sensorPacket_t sensor;

    void SetUp() override {
        sensor = sensorPacket_create('T', 30);
    }

    void TearDown() override {
        if (sensor != NULL)
            sensorPacket_destroy(&sensor);
    }
};

TEST_F(SensorDataTest, SensorPacketCreate) {
    ASSERT_NE(sensor, nullptr);
}

TEST_F(SensorDataTest, SensorPacketDestroy) {
    sensorPacket_destroy(&sensor);
    ASSERT_EQ(sensor, nullptr);
}

TEST_F(SensorDataTest, SensorPacketGetSensorName) {
    ASSERT_EQ(sensor->sensor, 'T');
}

TEST_F(SensorDataTest, SensorPacketGetData) {
    ASSERT_TRUE(sensorPacket_getData(sensor) == 30);
}