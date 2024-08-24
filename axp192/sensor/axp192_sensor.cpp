#include "axp192_sensor.h"
#include "esphome/core/log.h"

#define TAG this->get_component_source()

namespace esphome {
namespace axp192 {

void Axp192Sensor::dump_config() { LOG_SENSOR("Monitor: ", detail::to_hex(this->sensor_.value()).c_str(), this); }
void Axp192Sensor::setup() { this->get_parent()->register_sensor(sensor_.value(), this); }
float Axp192Sensor::get_setup_priority() const { return setup_priority::DATA; };
void Axp192Sensor::set_sensor(SensorType type) { sensor_ = type; };

}  // namespace axp192
}  // namespace esphome
