#pragma once

#include <map>
#include <unordered_map>
#include "../axp192_component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace axp192 {

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
class Axp192BinarySensor : public Parented<Axp192Component>, public binary_sensor::BinarySensor, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void set_sensor(MonitorType type);
  void set_sensor(IrqType type);
  float get_setup_priority() const override;

 private:
  esphome::optional<MonitorType> monitor_sensor_{};
  esphome::optional<IrqType> irq_sensor_{};
};

}  // namespace axp192
}  // namespace esphome
