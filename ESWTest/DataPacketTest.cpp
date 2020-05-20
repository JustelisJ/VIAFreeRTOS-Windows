#include "gtest/gtest.h"

extern "C" {
#include <DataPacket.h>
}

class DataPacketTest : public ::testing::Test {
protected:
    packet_t packet;

    void SetUp() override {
        packet = dataPacket_create(15, 50);
    }

    void TearDown() override {
        dataPacket_destoy(&packet);
    }
};

TEST_F(DataPacketTest, DataPacketCreate) {
    ASSERT_NE(packet, nullptr);
}

TEST_F(DataPacketTest, DataPacketDestroy) {
    dataPacket_destoy(&packet);
    ASSERT_EQ(packet, nullptr);
}

TEST_F(DataPacketTest, DataPacketGetTemperature) {
    ASSERT_EQ(dataPacket_getTemp(packet), 15);
}

TEST_F(DataPacketTest, DataPacketGetHumidity) {
    ASSERT_EQ(dataPacket_getHum(packet), 50);
}