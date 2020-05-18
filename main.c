#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Temperature.h"
#include "Humidity.h"

/* Priorities at which the tasks are created. */
#define TASK_TEMPERATURE_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define	TASK_HUMIDITY_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define TASK_CONTROLLER_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define TASK_LORADRIVER_PRIORITY			( tskIDLE_PRIORITY + 2 )
/* Task stack sizes*/
#define TASK_TEMPERATURE_STACK				( configMINIMAL_STACK_SIZE )
#define	TASK_HUMIDITY_STACK					( configMINIMAL_STACK_SIZE )
#define TASK_CONTROLLER_STACK				( configMINIMAL_STACK_SIZE )
#define TASK_LORADRIVER_STACK				( configMINIMAL_STACK_SIZE )

/* Task Handles */
TaskHandle_t _taskTemperatureHandle = NULL;
TaskHandle_t _taskHumidityHandle = NULL;

// --------------------------------------------------------------------------------------
void taskTemperature(void* pvParameters)
{
	temp_t temp = pvParameters;

	for (;;)
	{
		vTaskDelay(1000);
		temperature_measure(temp);
		printf("Temperature: %d\n", temperature_get(temp));
	}
}

// --------------------------------------------------------------------------------------
void taskHumidity(void* pvParameters)
{
	hum_t hum = pvParameters;

	for (;;)
	{
		vTaskDelay(1000);
		humidity_measure(hum);
		printf("Humidity: %d\n", humidity_get(hum));
	}
}

// --------------------------------------------------------------------------------------
void taskController(void* pvParameters)
{

}

// --------------------------------------------------------------------------------------
void taskLoraDriver(void* pvParameters)
{

}

// --------------------------------------------------------------------------------------
void main(void)
{
	temp_t temp = temperature_create();
	hum_t hum = humidity_create();
	/* Create the task, not storing the handle. */
	xTaskCreate(
		taskTemperature,       /* Function that implements the task. */
		"TemperatureTask",          /* Text name for the task. */
		TASK_TEMPERATURE_STACK,      /* Stack size in words, not bytes. */
		temp,    /* Parameter passed into the task. */
		TASK_TEMPERATURE_PRIORITY,/* Priority at which the task is created. */
		&_taskTemperatureHandle);      /* Used to pass out the created task's handle. */

		/* Create the task, storing the handle. */
	xTaskCreate(
		taskHumidity,       /* Function that implements the task. */
		"HumidityTask",          /* Text name for the task. */
		TASK_HUMIDITY_STACK,      /* Stack size in words, not bytes. */
		hum,    /* Parameter passed into the task. */
		TASK_HUMIDITY_STACK,/* Priority at which the task is created. */
		&_taskHumidityHandle);      /* Used to pass out the created task's handle. */

	// Let the operating system take over :)
	vTaskStartScheduler();
}