#pragma once
#include <stdint.h>

typedef struct SensorPacket
{
	char sensor;
	uint8_t data;
}SensorPacket;

typedef struct SensorPacket* sensorPacket_t;


sensorPacket_t sensorPacket_create(char sensor, uint8_t data);
void sensorPacket_destroy(sensorPacket_t* self);
char sensorPacket_getSensor(sensorPacket_t self);
uint8_t sensorPacket_getData(sensorPacket_t self);