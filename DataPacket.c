#include <stdlib.h>
#include "DataPacket.h"
#include <stdio.h>

packet_t dataPacket_create(uint8_t temp, uint8_t hum)
{
	packet_t packet = calloc(1, sizeof(DataPacket));

	if (packet == NULL)
		return NULL;

	packet->temp = temp;
	packet->hum = hum;
	return packet;
}

void dataPacket_destoy(packet_t* self)
{
	packet_t _self = *self;
	if (_self != NULL)
	{
		free(_self);
		_self = NULL;
		*self = _self;
	}
}

uint8_t dataPacket_getTemp(packet_t self)
{
	return self->temp;
}

uint8_t dataPacket_getHum(packet_t self)
{
	return self->hum;
}