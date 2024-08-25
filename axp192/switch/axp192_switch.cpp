#include "axp192_switch.h"
#include "esphome/core/log.h"

#define TAG this->get_component_source()

namespace esphome {
namespace axp192 {

float Axp192Switch::get_setup_priority() const { return setup_priority::HARDWARE; };

void Axp192Switch::set_output(OutputPin pin) { pin_ = pin; }

void Axp192Switch::dump_config() {
  ESP_LOGCONFIG(this->get_component_source(), "AXP192 Switch");
  switch_::log_switch(this->get_component_source(), "    ", detail::to_hex(pin_).c_str(), this);
}

void Axp192Switch::write_state(bool state) {
  ESP_LOGD(this->get_component_source(), "Set %s to %d", detail::to_hex(this->pin_).c_str(), state);
  switch (this->pin_) {
    case OutputPin::OUTPUT_LDO2:
      this->get_parent()->configure_ldo2(state);
      break;
    case OutputPin::OUTPUT_LDO3:
      this->get_parent()->configure_ldo3(state);
      break;
    case OutputPin::OUTPUT_DCDC1:
      this->get_parent()->configure_dcdc1(state);
      break;
    case OutputPin::OUTPUT_DCDC3:
      this->get_parent()->configure_dcdc3(state);
      break;
    case OutputPin::OUTPUT_LDOIO0:
      this->get_parent()->configure_ldoio0(state);
      break;
    default:
      break;
  }
}

void Axp192Switch::setup() { this->get_parent()->register_switch(this->pin_, this); }

}  // namespace axp192
}  // namespace esphome
