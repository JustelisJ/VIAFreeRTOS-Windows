#pragma once
#include <stdint.h>

typedef struct Humidity
{
	uint8_t lastValues;
}Humidity;

typedef struct Humidity* hum_t;

hum_t humidity_create();
void humidity_destroy(hum_t* self);
void humidity_measure(hum_t self);
uint8_t humidity_get(hum_t self);