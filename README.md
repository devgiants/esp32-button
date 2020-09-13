# ESP32 Button utility

This library provides a plain C helper to register and use a button, targeting ESP32 device with IDF (Iot Development Framework).

## Usage

[`main.c`](https://github.com/devgiants/esp32-button/blob/master/main/main.c) exposes some ways to use this. Basically, you have a `Button` struct for which you can define some mandatory values and some optional ones.
For optional, most of them will get some default on button creation.

## Mandatory
Example : 

```
 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "button.h"

static Button *button1;

// Callback with your actions when button is pressed
void buttonInputCallback(void *userArg) {
	ESP_LOGI("MAIN", "buttonInput from callback");
}

void app_main(void) {

	// Mandatory to initiates stuff needed for all buttons (queue...)
	buttonManagerInit();

	// Create button
	button1 = calloc( 1, sizeof(Button) );

	// Mandatory
	button1->gpio = 5;
	button1->pressInputTriggerCallback = buttonInputCallback;

	// Effective button creation
	initiateButton(button1);
}
```

## Optional

TODO
