#include "driver/gpio.h"
#include "esp_log.h"
#include "../include/button.h"

using namespace devgiants;


void IRAM_ATTR devgiants::Button::onPress(void* arg)
{
	ESP_LOGI("DEVGIANTS BUTTON", "trigger");
}

devgiants::Button::Button(int gpio, void (*inputTriggerCallback)(devgiants::Button *button)) : gpio(gpio) {
	this->inputTriggerCallback = inputTriggerCallback;
//	inputTriggerCallback(this);
	this->initiates();
};

void devgiants::Button::initiates() {
	ESP_LOGD("DEVGIANTS BUTTON", "Setup button on pin %d", this->gpio);
	gpio_num_t chosenGpio = (gpio_num_t)this->gpio;

//	gpio_pad_select_gpio(this->gpio);
//	gpio_set_direction(chosenGpio, GPIO_MODE_INPUT);
//
//	// TODO add exclusion for input only GPIO (no pull mode)
//	gpio_set_pull_mode(chosenGpio, (gpio_pull_mode_t)GPIO_PULLUP_ENABLE);
//
//	//install gpio isr service
//	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
//	//hook isr handler for specific gpio pin
//	gpio_isr_handler_add(chosenGpio, onPress, (void *)this);

	gpio_config_t io_conf;
	io_conf.intr_type = (gpio_int_type_t)GPIO_PIN_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pin_bit_mask = 1ULL << this->gpio;
	io_conf.pull_down_en = (gpio_pulldown_t)0;
	io_conf.pull_up_en = (gpio_pullup_t)1;
	gpio_config(&io_conf);

	gpio_set_intr_type(chosenGpio, GPIO_INTR_ANYEDGE);

	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
	gpio_isr_handler_add(chosenGpio, onPress, (void *)"test");
}

int devgiants::Button::getInputGpio() {
	return gpio;
}

