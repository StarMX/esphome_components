#include "template_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace template {

static const char *TAG = "LORA";

void LoraComponent::setup() {
    
}
void LoraComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "template:");
}

void LoraComponent::update() {
    ESP_LOGI(TAG, "template update");
}

void LoraComponent::loop() {
    this->data_received_callback_.call(message_);
}

void TemplateComponent::send(const std::string &message) {
    ESP_LOGV(TAG, "Send %s", message);
}

float LoraComponent::get_setup_priority() const { return setup_priority::HARDWARE; };
}
}