#pragma once

#include <map>
#include <unordered_map>
#include "../axp192_component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

namespace esphome {
namespace axp192 {
// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
class Axp192Switch : public Parented<Axp192Component>, public switch_::Switch, public Component {
 public:
  void dump_config() override;
  void setup() override;
  float get_setup_priority() const override;
  void set_output(OutputPin pin);

 protected:
  void write_state(bool state) override;
  OutputPin pin_ = OutputPin::OUTPUT_LDO2;
};
}  // namespace axp192
}  // namespace esphome
