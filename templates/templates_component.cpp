#include "templates_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace templates {

static const char *TAG = "templates";

void TemplatesComponent::setup() {
    message_="templates";
}
void TemplatesComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "templates:");
}

void TemplatesComponent::update() {
    ESP_LOGV(TAG, "templates update");
}

void TemplatesComponent::loop() {
    // this->data_received_callback_.call(message_);
}

void TemplatesComponent::send(const std::string &message) {
    ESP_LOGD(TAG, "Send %s", message.c_str());
}

float TemplatesComponent::get_setup_priority() const { return setup_priority::HARDWARE; };
}
}