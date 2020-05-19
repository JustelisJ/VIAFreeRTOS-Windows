#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Temperature.h"
#include "Humidity.h"
#include "event_groups.h"
#include <semphr.h>

/* Priorities at which the tasks are created. */
#define TASK_TEMPERATURE_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define	TASK_HUMIDITY_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define TASK_CONTROLLER_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define TASK_LORADRIVER_PRIORITY			( tskIDLE_PRIORITY + 1 )
/* Task stack sizes*/
#define TASK_TEMPERATURE_STACK				( configMINIMAL_STACK_SIZE + 200)
#define	TASK_HUMIDITY_STACK					( configMINIMAL_STACK_SIZE + 200)
#define TASK_CONTROLLER_STACK				( configMINIMAL_STACK_SIZE + 200)
#define TASK_LORADRIVER_STACK				( configMINIMAL_STACK_SIZE + 200)

/* Task Handles */
TaskHandle_t _taskTemperatureHandle = NULL;
TaskHandle_t _taskHumidityHandle = NULL;

/* Event group handle */
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)

EventGroupHandle_t xEventGroup;

/* Global mutax */

// --------------------------------------------------------------------------------------
void taskTemperature(void* pvParameters)
{
	temp_t temp = pvParameters;

	for (;;)
	{
		temperature_measure(temp);
		xEventGroupSetBits(xEventGroup, BIT_0);
		//printf("Temperature: %d\n", temperature_get(temp));
	}
}

// --------------------------------------------------------------------------------------
void taskHumidity(void* pvParameters)
{
	hum_t hum = pvParameters;

	for (;;)
	{
		humidity_measure(hum);
		xEventGroupSetBits(xEventGroup, BIT_1);
		//printf("Humidity: %d\n", humidity_get(hum));
	}
}

// --------------------------------------------------------------------------------------
void taskController(void* pvParameters)
{
	(void*)pvParameters;

	vTaskSuspend(_taskHumidityHandle);
	vTaskSuspend(_taskTemperatureHandle);

	for (;;)
	{
		printf("Controller\n");

		vTaskResume(_taskHumidityHandle);
		vTaskResume(_taskTemperatureHandle);

		EventBits_t uxBits = xEventGroupWaitBits(xEventGroup,  /* The event group being tested. */
			BIT_0 | BIT_1, /* The bits to wait for. */
			pdTRUE, /* BIT_0 & BIT_1 cleared before return*/
			pdTRUE, /* Wait for bits to be set */
			2000); /* Time out*/

		vTaskSuspend(_taskHumidityHandle);
		vTaskSuspend(_taskTemperatureHandle);

		if (uxBits == 3)	//0b11 uxBits value
		{
			printf("Sensor data retrieved\n");
			//TODO: message buffer for LoraDriver
		}
		else if (uxBits == 2)	//0b10 uxBits value
		{
			printf("Temperature not retrieved\n");
		}
		else if (uxBits == 1)	//0b01 uxBits
		{
			printf("Humidity not retrieved\n");
		}
		else
		{
			printf("Both sensors failed to set the bits");	//0b00 uxBits value
		}

	}
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

	/* Create the task. */
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
		TASK_HUMIDITY_PRIORITY,/* Priority at which the task is created. */
		&_taskHumidityHandle);      /* Used to pass out the created task's handle. */

	xTaskCreate(
		taskController,       /* Function that implements the task. */
		"ControllerTask",          /* Text name for the task. */
		TASK_CONTROLLER_STACK,      /* Stack size in words, not bytes. */
		NULL,    /* Parameter passed into the task. */
		TASK_CONTROLLER_PRIORITY,/* Priority at which the task is created. */
		NULL);      /* Used to pass out the created task's handle. */

	xTaskCreate(
		taskLoraDriver,       /* Function that implements the task. */
		"LoraDriverTask",          /* Text name for the task. */
		TASK_LORADRIVER_STACK,      /* Stack size in words, not bytes. */
		NULL,    /* Parameter passed into the task. */
		TASK_LORADRIVER_PRIORITY,/* Priority at which the task is created. */
		NULL);      /* Used to pass out the created task's handle. */

	xEventGroup = xEventGroupCreate();

	// Let the operating system take over :)
	vTaskStartScheduler();
}