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


bundle_t SensorBundle_create(temp_t temp, hum_t hum);
void SensorBundle_destroy(bundle_t self);

temp_t SensorBundle_getTemp(bundle_t self);
hum_t SensorBundle_getHum(bundle_t self);