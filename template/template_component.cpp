#include "template_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace template {

static const char *TAG = "TEMPLATE";

void TemplateComponent::setup() {
    
}
void TemplateComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "template:");
}

void TemplateComponent::update() {
    ESP_LOGI(TAG, "template update");
}

void TemplateComponent::loop() {
    this->data_received_callback_.call(message_);
}

void TemplateComponent::send(const std::string &message) {
    ESP_LOGV(TAG, "Send %s", message);
}

float TemplateComponent::get_setup_priority() const { return setup_priority::HARDWARE; };
}
}