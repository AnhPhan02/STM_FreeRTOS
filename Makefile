CC = arm-none-eabi-gcc
BUILD_DIR = build
KERNAL_DIR = FreeRTOS-Kernel
Driver_DIR = Drivers
BUILD_FLAG = -specs=nano.specs -specs=nosys.specs -mcpu=cortex-m4 -std=gnu11 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I$(KERNAL_DIR)/include -I$(KERNAL_DIR)/portable/GCC/ARM_CM4F -IDrivers/inc
LINK_FLAG = -T"STM32F411VETX_FLASH.ld" -Wl,-Map="build/FreeRTOS.map" -Wl,--gc-section -static

All:
	$(CC) -c main.c $(BUILD_FLAG) -o $(BUILD_DIR)/main.o
	$(CC) -c startup_stm32f411vetx.s $(BUILD_FLAG) -o $(BUILD_DIR)/startup.o
	$(CC) -c $(KERNAL_DIR)/list.c $(BUILD_FLAG) -o $(BUILD_DIR)/list.o
	$(CC) -c $(KERNAL_DIR)/queue.c $(BUILD_FLAG) -o $(BUILD_DIR)/queue.o
	$(CC) -c $(KERNAL_DIR)/stream_buffer.c $(BUILD_FLAG) -o $(BUILD_DIR)/stream_buffer.o
	$(CC) -c $(KERNAL_DIR)/tasks.c $(BUILD_FLAG) -o $(BUILD_DIR)/task.o
	$(CC) -c $(KERNAL_DIR)/timers.c $(BUILD_FLAG) -o $(BUILD_DIR)/timers.o
	$(CC) -c $(KERNAL_DIR)/croutine.c $(BUILD_FLAG) -o $(BUILD_DIR)/croutine.o
	$(CC) -c $(KERNAL_DIR)/event_groups.c $(BUILD_FLAG) -o $(BUILD_DIR)/event_groups.o
	$(CC) -c $(KERNAL_DIR)/portable/GCC/ARM_CM4F/port.c $(BUILD_FLAG) -o $(BUILD_DIR)/port.o
	$(CC) -c $(KERNAL_DIR)/portable/MemMang/heap_4.c $(BUILD_FLAG) -o $(BUILD_DIR)/heap_4.o
	$(CC) -c Drivers/Scr/led.c $(BUILD_FLAG) -o $(BUILD_DIR)/led.o
	$(CC) -c Drivers/Scr/printf_log.c $(BUILD_FLAG) -o $(BUILD_DIR)/printf_log.o
	$(CC) -c Drivers/Scr/ADC_temperature.c $(BUILD_FLAG) -o $(BUILD_DIR)/ADC_temperature.o
	$(CC) $(BUILD_DIR)/*.o $(BUILD_FLAG) $(LINK_FLAG) -o $(BUILD_DIR)/freeROTS_project.elf
	arm-none-eabi-objcopy -O ihex $(BUILD_DIR)/freeROTS_project.elf $(BUILD_DIR)/freeROTS_project.hex
load:
	"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" -c SWD -SE 0 3 -P $(BUILD_DIR)/freeROTS_project.hex -Run