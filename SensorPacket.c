#include "SensorPacket.h"
#include <stdlib.h>

sensorPacket_t sensorPacket_create(char sensor, uint8_t data)
{
	sensorPacket_t _sensor = calloc(1, sizeof(SensorPacket));

	if (sensor == NULL)
		return NULL;

	_sensor->sensor = sensor;
	_sensor->data = data;

	return _sensor;
}

void sensorPacket_destroy(sensorPacket_t* self)
{
	sensorPacket_t ss = *self;
	if (ss != NULL)
	{
		free(ss);
		ss = NULL;
		*self = ss;
	}
}

char sensorPacket_getSensor(sensorPacket_t self)
{
	return self->sensor;
}

uint8_t sensorPacket_getData(sensorPacket_t self)
{
	return self->data;
}
