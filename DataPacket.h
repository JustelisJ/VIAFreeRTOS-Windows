#pragma once
#include <stdint.h>

typedef struct DataPacket
{
	uint8_t temp;
	uint8_t hum;
}DataPacket;

typedef struct DataPacket* packet_t;


packet_t DataPacket_create(uint8_t temp, uint8_t hum);
void DataPacket_destoy(packet_t self);

uint8_t DataPacket_getTemp(packet_t self);
uint8_t DataPacket_getHum(packet_t self);