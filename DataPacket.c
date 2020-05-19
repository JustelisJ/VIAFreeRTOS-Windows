#include <stdlib.h>
#include "DataPacket.h"

packet_t DataPacket_create(uint8_t temp, uint8_t hum)
{
	packet_t packet = calloc(1, sizeof(packet_t));

	if (packet == NULL)
		return NULL;

	packet->temp = temp;
	packet->hum = hum;
	return packet;
}

void DataPacket_destoy(packet_t self)
{
	if (self->hum != 221 && self->temp != 221)
	{
		self->hum = NULL;
		self->temp = NULL;
		free(self);
	}
}

uint8_t DataPacket_getTemp(packet_t self)
{
	return self->temp;
}

uint8_t DataPacket_getHum(packet_t self)
{
	return self->hum;
}
