#include<stdint.h>
#include"FreeRTOS.h"
#include"task.h"
#include"led.h"
#include"printf_log.h"
#include"ADC_temperature.h"
#include"queue.h"
#include"event_groups.h"
#include"semphr.h"

#define READ_TEMPERATURE_COMPLETE_EVENT (1 << 0)
#define READ_TEMPERATURE_COMPLETE_ERROR	(1 << 1)

void vApplicationMallocFailedHook()
{

}

void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                        StackType_t ** ppxIdleTaskStackBuffer,
                                        uint32_t * pulIdleTaskStackSize)
{

}

void vApplicationTickHook()
{

}

void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                        char * pcTaskName)
{

}

void vApplicationIdleHook()
{

}

void vApplicationGetTimerTaskMemory()
{

}
void SystemInit()
{

}

void func1(void* parame)
{
	while(1)
	{
		Leds_ctrl(12,1);
		vTaskDelay(500);
		Leds_ctrl(12,0);
		vTaskDelay(500);
	}
}

void func2(void* parame)
{
	while(1)
	{
		Leds_ctrl(14,1);
		vTaskDelay(1500);
		Leds_ctrl(14,0);
		vTaskDelay(1500);
	}
}

xQueueHandle xTemperature;
EventGroupHandle_t xTemperature_Event;
QueueHandle_t xLock_Mutex;
void Send_Temp(void* parame)
{
	while(1)
	{
		//todo: send data from Queue

		//todo: wait event complete
		xEventGroupWaitBits(xTemperature_Event,
							READ_TEMPERATURE_COMPLETE_EVENT,
							pdTRUE,
							pdFAIL,
							10000);
		float recive_temp;
		UBaseType_t num_data = uxQueueMessagesWaiting(xTemperature);
		xSemaphoreTake(xLock_Mutex, 1000);				//take key

		printf_Log("Temperature: [");
		for(int i = 0; i < num_data; i++)
		{
		xQueueReceive(xTemperature, &recive_temp, 10000);
		printf_Log("%d.%d, ", (int)recive_temp, (int)((recive_temp - (int)recive_temp)*100));
		}
		
		printf_Log("\b\b]\r\n");
		xSemaphoreGive(xLock_Mutex);					//release key
	}
}

void Read_Temp(void* parame)
{	
	//todo: Put data into Queue
	
	int cnt = 0;
	//todo: cout = 10 trigger data to xQueueRecive
	while(1)
	{
		float temper_sensor = temp_measure();
		
		xQueueSend(xTemperature, &temper_sensor, 10000);
		vTaskDelay(100);
		if(cnt++ >= 10)
		{
			cnt = 0;
			/*send event*/
			xEventGroupSetBits(xTemperature_Event, READ_TEMPERATURE_COMPLETE_EVENT );
		}
	}
}

void Func5(void* parame)
{
	while(1)
	{	
		xSemaphoreTake(xLock_Mutex, 1000);
		printf_Log("[%d] - Time process\r\n",xTaskGetTickCount());
		xSemaphoreGive(xLock_Mutex);
		vTaskDelay(1000);
	}
}

void main()
{
	Leds_init();
	UART_init();
	adc_init();

	TaskHandle_t task1;
	TaskHandle_t task2;
	TaskHandle_t task3;
	TaskHandle_t task4;
	TaskHandle_t task5;
	//todo: Task create
	xTaskCreate(func1, "task 1", 1024,NULL, 0, &task1);	
	xTaskCreate(func2, "task 2",1024, NULL,1, &task2);
	xTaskCreate(Send_Temp, "task 3",1024, NULL,1, &task3);
	xTaskCreate(Read_Temp, "task 4", 1024, NULL,1, &task4);
	xTaskCreate(Func5, "task 5", 1028, NULL,1, &task5);
	//todo: Create Queue
	xTemperature = xQueueCreate(
							128,
							sizeof(float)
							);
	//todo: Create Event
	xTemperature_Event = xEventGroupCreate();
	//todo: Create Mutex
	xLock_Mutex = xSemaphoreCreateMutex();
	vTaskStartScheduler();
	while(1)	
	{
		
	}
}