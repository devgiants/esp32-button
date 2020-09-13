#ifndef DEF_DEVGIANTS_BUTTON
#define DEF_DEVGIANTS_BUTTON

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define MODULE_LOG_TAG "DEVGIANTS_BUTTON"

#define ESP_INTR_FLAG_DEFAULT 0
#define DEFAULT_DEBOUNCING_DELAY_MS 100

typedef void (*PressInputTriggerCallback)(void* button);
typedef void (*LongPressInputTriggerCallback)(void* button);

typedef enum {
	PRESS = 0,
	LONG_PRESS = 1,
	RELEASE = 2
} buttonEvents;

typedef struct {
	uint32_t debounceDelay;
	uint32_t debounceTimestamp;
	buttonEvents enabledEvents[5];
	int gpio;
	PressInputTriggerCallback pressInputTriggerCallback;
	LongPressInputTriggerCallback longPressInputTriggerCallback;
	gpio_int_type_t interruptType;
	gpio_pullup_t pullUp;
	gpio_pulldown_t pullDown;
} Button;

void buttonManagerInit();
void initiateButton(Button *button);

#endif
