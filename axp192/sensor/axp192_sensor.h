#pragma once

#include <map>
#include <unordered_map>
#include "../axp192_component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace axp192 {

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
class Axp192Sensor : public Parented<Axp192Component>, public sensor::Sensor, public Component {
 public:
  void dump_config() override;
  void setup() override;
  float get_setup_priority() const override;
  void set_sensor(SensorType type);

 private:
  esphome::optional<SensorType> sensor_{};
};
}  // namespace axp192
}  // namespace esphome
