#include "Humidity.h"

hum_t humidity_create()
{
	hum_t _temp = calloc(1, sizeof(hum_t));

	if (_temp == NULL)
	{
		return NULL;
	}

	_temp->lastValues = NULL;

	return _temp;
}

void humidity_destroy(hum_t self)
{
	if (self != NULL)
	{
		free(self);
	}
	self = NULL;
}

void humidity_measure(hum_t self)
{
	self->lastValues = getRandomValue();

}

uint8_t humidity_get(hum_t self)
{
	return self->lastValues;
}

static int getRandomValue()
{
	//Return a value from 0 to 100

	return rand() % 100;
}