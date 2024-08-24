#pragma once

#include <map>
#include <unordered_map>
#include "../axp192_component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/component.h"

namespace esphome {
namespace axp192 {

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
class Axp192Output : public Parented<Axp192Component>, public output::FloatOutput, public Component {
 public:
  void dump_config() override;
  void setup() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; };
  void set_output(OutputPin pin) { pin_ = pin; }
  void set_voltage(float voltage) { voltage_ = voltage; }
  void write_state(float state) override;

 private:
  OutputPin pin_ = OutputPin::OUTPUT_LDO2;
  esphome::optional<float> voltage_;
};



}  // namespace axp192
}  // namespace esphome
