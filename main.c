#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Temperature.h"
#include "Humidity.h"
#include "DataPacket.h"
#include "SensorPacket.h"
#include "event_groups.h"
#include <semphr.h>
#include <message_buffer.h>

/* Priorities at which the tasks are created. */
#define TASK_TEMPERATURE_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define	TASK_HUMIDITY_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define TASK_CONTROLLER_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define TASK_LORADRIVER_PRIORITY			( tskIDLE_PRIORITY )
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
SemaphoreHandle_t Mutex_Printf;

/*Buffer declaration*/
QueueHandle_t xQueueLora;
QueueHandle_t xQueueSensors;

// --------------------------------------------------------------------------------------
void taskTemperature(void* pvParameters)
{
	temp_t temp = pvParameters;
	sensorPacket_t packet;

	for (;;)
	{
		temperature_measure(temp);
		packet = sensorPacket_create('T', temperature_get(temp));
		xQueueSend(xQueueSensors, &packet, portMAX_DELAY);
		xEventGroupSetBits(xEventGroup, BIT_0);
		//printf("Temperature: %d\n", temperature_get(temp));
		vTaskDelay(500);
	}
}

// --------------------------------------------------------------------------------------
void taskHumidity(void* pvParameters)
{
	hum_t hum = pvParameters;
	sensorPacket_t packet;

	for (;;)
	{
		humidity_measure(hum);
		packet = sensorPacket_create('H', humidity_get(hum));
		xQueueSend(xQueueSensors, &packet, portMAX_DELAY);
		xEventGroupSetBits(xEventGroup, BIT_1);
		//printf("Humidity: %d\n", humidity_get(hum));
		vTaskDelay(500);
	}
}

// --------------------------------------------------------------------------------------
void taskController(void* pvParameters)
{
	vTaskSuspend(_taskHumidityHandle);
	vTaskSuspend(_taskTemperatureHandle);

	for (;;)
	{
		/*xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
		printf("Controller\n");
		xSemaphoreGive(Mutex_Printf);*/

		vTaskResume(_taskHumidityHandle);
		vTaskResume(_taskTemperatureHandle);

		EventBits_t uxBits = xEventGroupWaitBits(xEventGroup,  /* The event group being tested. */
			BIT_0 | BIT_1, /* The bits to wait for. */
			pdTRUE, /* BIT_0 & BIT_1 cleared before return*/
			pdTRUE, /* Wait for bits to be set */
			3000); /* Time out*/

		vTaskSuspend(_taskHumidityHandle);
		vTaskSuspend(_taskTemperatureHandle);

		if ((uxBits & (BIT_0 | BIT_1)) == (BIT_0 | BIT_1))	//0b11 uxBits value
		{
			xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
			printf("Sensor data retrieved");
			xSemaphoreGive(Mutex_Printf);

			sensorPacket_t sPacket;
			uint8_t temp = 0;
			uint8_t hum = 0;

			int i = 0;
			while (i != 2)
			{
				BaseType_t em = xQueueReceive(xQueueSensors, &sPacket, 0);
				if (em == pdTRUE)
				{
					if (sPacket->sensor == 'T')
					{
						temp = sPacket->data;
					}
					else
					{
						hum = sPacket->data;
					}
				}
				else
				{
					i--;
				}
				i++;
			}

			packet_t dataPacket = dataPacket_create(temp, hum);
			xQueueSend(xQueueLora, &dataPacket, portMAX_DELAY);
			xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
			printf("\tSensor data sent to Lora\n");
			xSemaphoreGive(Mutex_Printf);
		}
		else if ((uxBits & BIT_1) == BIT_1)	//0b10 uxBits value
		{
			xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
			printf("Temperature not retrieved\n");
			xSemaphoreGive(Mutex_Printf);
			xQueueReset(xQueueSensors);
		}
		else if ((uxBits & BIT_0) == BIT_0)	//0b01 uxBits
		{
			xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
			printf("Humidity not retrieved\n");
			xSemaphoreGive(Mutex_Printf);
			xQueueReset(xQueueSensors);
		}
		else if(uxBits == 0)
		{
			xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
			printf("Both sensors failed to set the bits\n");	//0b00 uxBits value
			xSemaphoreGive(Mutex_Printf);
			xQueueReset(xQueueSensors);
		}
		else
		{
			xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
			printf("Something went wrong. EventGroup unexpected bits set.\n");
			xSemaphoreGive(Mutex_Printf);
			xQueueReset(xQueueSensors);
		}
		
		vTaskDelay(1000);
	}
}

// --------------------------------------------------------------------------------------
void taskLoraDriver(void* pvParameters)
{
	packet_t packet = NULL;
	for (;;)
	{
		BaseType_t em = xQueueReceive(xQueueLora, &packet, 0);

		if (em == pdTRUE)
		{
			xSemaphoreTake(Mutex_Printf, portMAX_DELAY);
			printf("Temperature: %d, Humidity: %d\n", packet->temp, packet->hum);
			xSemaphoreGive(Mutex_Printf);
		}
	}
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

	xQueueLora = xQueueCreate(10, sizeof(packet_t));
	xQueueSensors = xQueueCreate(10, sizeof(sensorPacket_t));

	Mutex_Printf = xSemaphoreCreateMutex();

	// Let the operating system take over :)
	vTaskStartScheduler();
}