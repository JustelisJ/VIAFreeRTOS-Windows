#include <stdlib.h>
#include "Temperature.h"

static uint8_t getRandomValue()
{
	//Returns a value between 1 and 50
	return rand() % 50 + 1;
}

temp_t temperature_create()
{
	temp_t _temp = calloc(1, sizeof(Temperature));

	if (_temp == NULL)
	{
		return NULL;
	}

	_temp->lastValues = NULL;

	return _temp;
}

void temperature_destroy(temp_t* self)
{
	temp_t _self = *self;
	if (_self != NULL)
	{
		free(_self);
		_self = NULL;
		*self = _self;
	}
}

void temperature_measure(temp_t self)
{
	self->lastValues = getRandomValue();
}

uint8_t temperature_get(temp_t self)
{
	return self->lastValues;
}