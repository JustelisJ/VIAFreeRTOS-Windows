#include "gtest/gtest.h"

extern "C" {
#include <DataPacket.h>
}

class DataPacketTest : public ::testing::Test {
protected:
    packet_t packet;

    void SetUp() override {
        packet = DataPacket_create(15, 50);
    }

    void TearDown() override {
        DataPacket_destoy(packet);
    }
};

TEST_F(DataPacketTest, DataPacketCreate) {
    ASSERT_NE(packet, nullptr);
}

TEST_F(DataPacketTest, DataPacketDestroy) {
    DataPacket_destoy(packet);
    ASSERT_EQ(packet->hum, 221);
    ASSERT_EQ(packet->temp, 221);
}

TEST_F(DataPacketTest, DataPacketGetTemperature) {
    ASSERT_EQ(DataPacket_getTemp(packet), 15);
}

TEST_F(DataPacketTest, DataPacketGetHumidity) {
    ASSERT_EQ(DataPacket_getHum(packet), 50);
}