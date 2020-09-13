#include "../include/button.h"

static xQueueHandle gpioEventQueue = NULL;
static bool isMandatoryParamsFilled(Button *button);
static void defaultValuesAssignment(Button *button);
static void physicalButtonConfiguration(Button *button);
static bool isDebounced(Button *button);

// TODO check in depth why IRAM
static void IRAM_ATTR buttonInputCallback(void *userArg) {
	// TODO check without & because it looks like pointer of pointer
	xQueueSendFromISR(gpioEventQueue, &userArg, NULL);
}

static void gpioDebouncingTask(void *arg) {
	for (;;) {
		if (xQueueReceive(gpioEventQueue, &arg, portMAX_DELAY)) {
			Button *buttonPressed = (Button*) arg;

			if(isDebounced(buttonPressed))
			{
				ESP_LOGD("DEVGIANTS BUTTON", "Button pressed GPIO number : %d",
									buttonPressed->gpio);
				buttonPressed->pressInputTriggerCallback(buttonPressed);
			}
		}
	}
}

void buttonManagerInit() {
	gpioEventQueue = xQueueCreate(10, sizeof(Button*));
	// TODO check how to determine stack depth?
	xTaskCreate(gpioDebouncingTask, "gpio_debouncing_task", 16384, NULL, 10,
			NULL);
}

void initiateButton(Button *button) {
	if(!isMandatoryParamsFilled(button))
	{
		return;
	}
	defaultValuesAssignment(button);
	physicalButtonConfiguration(button);
}

static bool isMandatoryParamsFilled(Button *button)
{
	if(0 == button->gpio)
	{
		ESP_LOGE(MODULE_LOG_TAG, "Missing GPIO for button assignment");
		return false;
	}

	if(0 == button->pressInputTriggerCallback)
	{
		ESP_LOGE(MODULE_LOG_TAG, "Missing normal press event callback");
		return false;
	}

	return true;
}

static void defaultValuesAssignment(Button *button)
{
	if(button->debounceDelay == 0) {
		button->debounceDelay = DEFAULT_DEBOUNCING_DELAY_MS;
	}

	if(button->interruptType == 0) {
		button->interruptType = (gpio_int_type_t)GPIO_INTR_NEGEDGE;
	}

	if(button->pullUp == 0) {
		button->pullUp = (gpio_pullup_t)GPIO_PULLUP_ENABLE;
	}

	if(button->pullDown == 0) {
		button->pullDown = (gpio_pulldown_t)GPIO_PULLDOWN_DISABLE;
	}
//	if(button->enabledEvents[0])
}

static void physicalButtonConfiguration(Button *button)
{
	gpio_config_t io_conf;
	io_conf.intr_type = button->interruptType;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = 1ULL << (gpio_num_t)button->gpio;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_up_en = button->pullUp;
	io_conf.pull_down_en = button->pullDown;
	gpio_config(&io_conf);

	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	gpio_isr_handler_add((gpio_num_t)button->gpio, buttonInputCallback, (void*) button);
}

static bool isDebounced(Button *button)
{
	uint32_t currentTimestamp = esp_log_timestamp();
	bool isDebounced = false;

	if(button->debounceTimestamp == 0) {
		button->debounceTimestamp = currentTimestamp;
	}

	ESP_LOGD(MODULE_LOG_TAG, "Current time is %d. Previous timestamp was %d", currentTimestamp, button->debounceTimestamp);

	if((currentTimestamp - button->debounceTimestamp) > button->debounceDelay) {
		isDebounced = true;
	}

	button->debounceTimestamp = currentTimestamp;

	return isDebounced;
}
