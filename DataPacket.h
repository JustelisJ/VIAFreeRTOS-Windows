#pragma once
#include <stdint.h>

typedef struct DataPacket
{
	uint8_t temp;
	uint8_t hum;
}DataPacket;

typedef struct DataPacket* packet_t;


packet_t dataPacket_create(uint8_t temp, uint8_t hum);
void dataPacket_destoy(packet_t* self);
uint8_t dataPacket_getTemp(packet_t self);
uint8_t dataPacket_getHum(packet_t self);