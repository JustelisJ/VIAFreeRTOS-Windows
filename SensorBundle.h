#pragma once
#include <stdint.h>
#include <Temperature.h>
#include <Humidity.h>

typedef struct SensorBundle
{
	temp_t temp;
	hum_t hum;
}SensorBundle;

typedef struct SensorBundle* bundle_t;


bundle_t sensorBundle_create(temp_t temp, hum_t hum);
void sensorBundle_destroy(bundle_t* self);
temp_t sensorBundle_getTemp(bundle_t self);
hum_t sensorBundle_getHum(bundle_t self);