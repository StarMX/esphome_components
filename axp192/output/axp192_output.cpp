#include "axp192_output.h"
#include "esphome/core/log.h"

#define TAG this->get_component_source()

namespace esphome {
namespace axp192 {


void Axp192Output::dump_config() {
  ESP_LOGCONFIG(this->get_component_source(), "Published Output:");
  LOG_FLOAT_OUTPUT(this);
}

void Axp192Output::setup() { this->get_parent()->register_output(this->pin_, this); }

void Axp192Output::write_state(float state) {
  switch (this->pin_) {
    case OutputPin::OUTPUT_LDO2:
      this->get_parent()->configure_ldo2(state != 0.0f);
      if (state != 1.0f) {
        this->get_parent()->configure_ldo2_voltage(state);
      }
      break;
    case OutputPin::OUTPUT_LDO3:
      this->get_parent()->configure_ldo3(state != 0.0f);
      if (state != 1.0f) {
        this->get_parent()->configure_ldo3_voltage(state);
      }
      break;
    case OutputPin::OUTPUT_DCDC1:
      this->get_parent()->configure_dcdc1(state != 0.0f);
      if (state != 1.0f)

      {
        this->get_parent()->configure_dcdc1_voltage(state);
      }
      break;
    case OutputPin::OUTPUT_DCDC3:
      this->get_parent()->configure_dcdc3(state != 0.0f);
      if (state != 1.0f)

      {
        this->get_parent()->configure_dcdc3_voltage(state);
      }
      break;
    case OutputPin::OUTPUT_LDOIO0:
      this->get_parent()->configure_ldoio0(state != 0.0f);
      if (state != 1.0f) {
        this->get_parent()->configure_ldoio0_voltage(state);
      }
      break;
    default:
      break;
  }
}


}  // namespace axp192
}  // namespace esphome
