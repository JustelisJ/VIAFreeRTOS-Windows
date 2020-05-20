#include <stdlib.h>
#include "SensorBundle.h"

bundle_t sensorBundle_create(temp_t temp, hum_t hum)
{
	bundle_t bundle = malloc(sizeof(SensorBundle));
	if (bundle == NULL)
		return NULL;

	bundle->temp = temp;
	bundle->hum = hum;

	return bundle;
}

void sensorBundle_destroy(bundle_t* self)
{
	bundle_t _self = *self;
	if (_self != NULL)
	{
		hum_t hum = _self->hum;
		temp_t temp = _self->temp;
		humidity_destroy(&hum);
		temperature_destroy(&temp);
		free(_self);
		_self = NULL;
		*self = _self;
	}
}

temp_t sensorBundle_getTemp(bundle_t self)
{
	return self->temp;
}

hum_t sensorBundle_getHum(bundle_t self)
{
	return self->hum;
}