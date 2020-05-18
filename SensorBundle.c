#include <stdlib.h>
#include "SensorBundle.h"

bundle_t SensorBundle_create(temp_t temp, hum_t hum)
{
	bundle_t bundle = calloc(1, sizeof(bundle_t));
	if (bundle == NULL)
		return NULL;

	bundle->temp = temp;
	bundle->hum = hum;

	return bundle;
}

void SensorBundle_destroy(bundle_t self)
{
	if (self != NULL)
	{
		free(self);
	}
	self = NULL;
}

temp_t SensorBundle_getTemp(bundle_t self)
{
	return self->temp;
}

hum_t SensorBundle_getHum(bundle_t self)
{
	return self->hum;
}