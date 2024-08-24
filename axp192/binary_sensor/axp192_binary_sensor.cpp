#include "axp192_binary_sensor.h"
#include "esphome/core/log.h"

#define TAG this->get_component_source()

namespace esphome {
namespace axp192 {
void Axp192BinarySensor::dump_config() {
  if (this->irq_sensor_.has_value()) {
    LOG_BINARY_SENSOR("IRQ", detail::to_hex(this->irq_sensor_.value()).c_str(), this);
  }
  if (this->monitor_sensor_.has_value()) {
    LOG_BINARY_SENSOR("Monitor: ", detail::to_hex(this->monitor_sensor_.value()).c_str(), this);
  }
}

void Axp192BinarySensor::setup() {
  if (this->irq_sensor_.has_value()) {
    this->get_parent()->register_irq(this->irq_sensor_.value(), this);
  }
  if (this->monitor_sensor_.has_value()) {
    this->get_parent()->register_monitor(this->monitor_sensor_.value(), this);
  }
}

void Axp192BinarySensor::set_sensor(MonitorType type) { monitor_sensor_ = type; };

void Axp192BinarySensor::set_sensor(IrqType type) { irq_sensor_ = type; };

float Axp192BinarySensor::get_setup_priority() const { return setup_priority::DATA; };

}  // namespace axp192
}  // namespace esphome
