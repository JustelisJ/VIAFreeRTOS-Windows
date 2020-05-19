#include <stdlib.h>
#include "Temperature.h"

temp_t temperature_create()
{
	temp_t _temp = calloc(1, sizeof(temp_t));

	if (_temp == NULL)
	{
		return NULL;
	}

	_temp->lastValues = NULL;

	return _temp;
}

void temperature_destroy(temp_t self)
{
	if (self->lastValues != NULL)
	{
		free(self);
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

static int getRandomValue()
{
	//Return a value from -50 to 50
	int positive = rand() % 2;

	int number = rand() % 50;

	if (positive == 0)
		number = number * -1;

	return number;
}