#pragma once
#include <stdint.h>

typedef struct Temperature
{
	uint8_t lastValues;
}Temperature;

typedef struct Temperature* temp_t;

temp_t temperature_create();
void temperature_destroy(temp_t self);
void temperature_measure(temp_t self);
uint8_t temperature_get(temp_t self);