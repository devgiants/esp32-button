#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "button.h"

//static Button button1;
static Button *button1;

void buttonInputCallback(void *userArg) {
	ESP_LOGI("MAIN", "buttonInput from callback");
}

void app_main(void) {
	buttonManagerInit();

	// Handle first button
	button1 = calloc( 1, sizeof(Button) );
	button1->gpio = 5;
	button1->pressInputTriggerCallback = buttonInputCallback;
	initiateButton(button1);
}

