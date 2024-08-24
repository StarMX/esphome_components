#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

#include <map>
#include <unordered_map>

namespace esphome {
namespace axp192 {

class Axp192BinarySensor;
class Axp192Sensor;
class Axp192Switch;
class Axp192Output;
namespace detail {
template<typename T, T InMin, T InMax, uint8_t OutMin, uint8_t OutMax> uint8_t constrained_remap(T value) {
  return uint8_t(remap<T, T>(clamp<T>(value, InMin, InMax), InMin, InMax, OutMin, OutMax));
}

static constexpr size_t find_register_bit(size_t size, size_t index, size_t bit) {
  return 1U << (bit + ((size - index - 1) * 8));
}

// NOLINTNEXTLINE(fuchsia-trailing-return)
template<typename E> static constexpr auto to_int(const E value) -> typename std::underlying_type<E>::type {
  return static_cast<typename std::underlying_type<E>::type>(value);
}

template<typename E> static std::string to_hex(const E value) {
  return format_hex_pretty(static_cast<typename std::underlying_type<E>::type>(value));
}

}  // namespace detail

// 9.11.1
enum class RegisterLocations : uint8_t {
  POWER_SUPPLY_STATUS = 0x00,
  POWER_SUPPLY_CHARGE_STATUS = 0x01,
  ACIN_VBUS_STATUS = 0x04,
  RTC_DATA_BUFFER = 0x06,
  EXTEN_DCDC2_CONTROL = 0x10,
  DCDC13_LDO23_CONTROL = 0x12,
  DCDC2_VOLTAGE = 0x23,
  DCDC2_VOLTAGE_SLOPE = 0x25,
  DCDC1_VOLTAGE = 0x26,
  DCDC3_VOLTAGE = 0x27,
  LDO23_VOLTAGE = 0x28,
  VBUS_IPSOUT_ACCESS = 0x30,
  VOFF_VOLTAGE = 0x31,
  POWEROFF_BATTERY_CHLED_CONTROL = 0x32,
  CHARGE_CONTROL_REG1 = 0x33,
  CHARGE_CONTROL_REG2 = 0x34,
  BATTERY_BACKUP_CONTROL = 0x35,
  PEK_PARAMETERS = 0x36,
  DCDC_SWITCH_FREQUENCY = 0x37,
  BATTERY_CHARGE_UNDERTEMP_VALUE = 0x38,
  BATTERY_CHARGE_OVERTEMP_VALUE = 0x39,
  APS_UNDERVOLTAGE_UNDERVOLTAGE_LEVEL1 = 0x3A,
  APS_UNDERVOLTAGE_UNDERVOLTAGE_LEVEL2 = 0x3B,
  BATTERY_DISCHARGE_UNDERTEMP_VALUE = 0x3C,
  BATTERY_DISCHARGE_OVERTEMP_VALUE = 0x3D,
  DCDC_MODE = 0x80,
  ADC_ENABLE_REGISTER1 = 0x82,
  ADC_ENABLE_REGISTER2 = 0x83,
  ADC_SAMPLE_FREQUENCY_TS_CONTROL = 0x84,
  GPIO_30_INPUT_RANGE = 0x85,
  TIMER_CONTROL = 0x8A,
  VBUS_MONITOR_SETTINGS = 0x8B,
  OVERTEMP_POWEROFF_CONTROL = 0x8F,
  GPIO_CONTROL = 0x90,
  GPIO_LDO_VOLTAGE = 0x91,
  GPIO1_CONTROL = 0x92,
  GPIO2_CONTROL = 0x93,
  GPIO20_SIGNAL_STATUS = 0x94,
  GPIO43_FUNCTION_CONTROL = 0x95,
  GPIO43_SIGNAL_STATUS = 0x96,
  GPIO20_PULLDOWN = 0x96,
  PWM1_FREQUENCY = 0x98,
  PWM1_DUTY_CYCLE1 = 0x99,
  PWM1_DUTY_CYCLE2 = 0x9A,
  PWM2_FREQUENCY = 0x9B,
  PWM2_DUTY_CYCLE1 = 0x9C,
  PWM2_DUTY_CYCLE2 = 0x9D,
  GPIO5_CONTROL = 0x9E,
  IRQ_ENABLE_REGISTER1 = 0x40,
  IRQ_ENABLE_REGISTER2 = 0x41,
  IRQ_ENABLE_REGISTER3 = 0x42,
  IRQ_ENABLE_REGISTER4 = 0x43,
  IRQ_STATUS_REGISTER1 = 0x44,
  IRQ_STATUS_REGISTER2 = 0x45,
  IRQ_STATUS_REGISTER3 = 0x46,
  IRQ_STATUS_REGISTER4 = 0x47,
  ACIN_VOLTAGE_HIGH8 = 0x56,
  ACIN_VOLTAGE_LOW4 = 0x57,
  ACIN_CURRENT_HIGH8 = 0x58,
  ACIN_CURRENT_LOW4 = 0x59,
  VBUS_VOLTAGE_HIGH8 = 0x5A,
  VBUS_VOLTAGE_LOW4 = 0x5B,
  VBUS_CURRENT_HIGH8 = 0x5C,
  VBUS_CURRENT_LOW4 = 0x5D,
  AXP_TEMP_HIGH8 = 0x5E,
  AXP_TEMP_LOW4 = 0x5F,
  BATTERY_TEMP_HIGH8 = 0x62,
  BATTERY_TEMP_LOW4 = 0x63,
  GPIO0_VOLTAGE_HIGH8 = 0x64,
  GPIO0_VOLTAGE_LOW4 = 0x65,
  GPIO1_VOLTAGE_HIGH8 = 0x66,
  GPIO1_VOLTAGE_LOW4 = 0x67,
  GPIO2_VOLTAGE_HIGH8 = 0x68,
  GPIO2_VOLTAGE_LOW4 = 0x69,
  GPIO3_VOLTAGE_HIGH8 = 0x6A,
  GPIO3_VOLTAGE_LOW4 = 0x6B,
  BATTERY_POWER_HIGH8 = 0x70,
  BATTERY_POWER_MIDDLE8 = 0x71,
  BATTERY_POWER_LOW8 = 0x72,
  BATTERY_VOLTAGE_HIGH8 = 0x78,
  BATTERY_VOLTAGE_LOW4 = 0x79,
  BATTERY_CHARGE_CURRENT_HIGH8 = 0x7A,
  BATTERY_CHARGE_CURRENT_LOW5 = 0x7B,
  BATTERY_DISCHARGE_CURRENT_HIGH8 = 0x7C,
  BATTERY_DISCHARGE_CURRENT_LOW5 = 0x7D,
  APS_VOLTAGE_HIGH8 = 0x7E,
  APS_VOLTAGE_LOW4 = 0x7F,
  BATTERY_CHARGE_COULUMB_COUNTER3 = 0xB0,
  BATTERY_CHARGE_COULUMB_COUNTER2 = 0xB1,
  BATTERY_CHARGE_COULUMB_COUNTER1 = 0xB2,
  BATTERY_CHARGE_COULUMB_COUNTER0 = 0xB3,
  BATTERY_DISCHARGE_COULUMB_COUNTER3 = 0xB4,
  BATTERY_DISCHARGE_COULUMB_COUNTER2 = 0xB5,
  BATTERY_DISCHARGE_COULUMB_COUNTER1 = 0xB6,
  BATTERY_DISCHARGE_COULUMB_COUNTER0 = 0xB7,
  BATTERY_COULUMB_COUTER_CONTROL = 0xB8
};

enum class OutputPin : uint8_t { OUTPUT_LDO2, OUTPUT_LDO3, OUTPUT_DCDC1, OUTPUT_DCDC3, OUTPUT_LDOIO0 };

// 32bit int - <reg 00><reg 1> MSB -> LSB

enum class MonitorType : uint32_t {
  // reg 0x00 - 9.11.5
  ACIN_PRESENT = detail::find_register_bit(2, 0, 7),
  ACIN_VALID = detail::find_register_bit(2, 0, 6),
  VBUS_PRESENT = detail::find_register_bit(2, 0, 5),
  VBUS_VALID = detail::find_register_bit(2, 0, 4),
  VBUS_ABOVE = detail::find_register_bit(2, 0, 3),
  BATTERY_CURRENT_DIRECTION = detail::find_register_bit(2, 0, 2),
  ACIN_VBUS_SHORT = detail::find_register_bit(2, 0, 1),
  ACIN_VBUS_TRIGGER_BOOT = detail::find_register_bit(2, 0, 0),
  // reg 0x01 - 9.11.6
  AXP_OVER_TEMP = detail::find_register_bit(2, 1, 7),
  CHARGE_INDICATE = detail::find_register_bit(2, 1, 6),
  BATTERY_PRESENT = detail::find_register_bit(2, 1, 5),
  BATTERY_ACTIVE = detail::find_register_bit(2, 1, 3),
  CHARGE_CURRENT_LOW = detail::find_register_bit(2, 1, 2)
};
// 42 - 6)
//  32bit int - <reg 40><reg 41><reg 43><reg 43> MSB -> LSB
enum class IrqType : uint32_t {
  // reg RegisterLocations::IRQ_STATUS_REGISTER1 - 9.11.53
  ACIN_OVER_VOLTAGE = detail::find_register_bit(4, 0, 7),
  ACIN_INSERTED = detail::find_register_bit(4, 0, 6),
  ACIN_REMOVED = detail::find_register_bit(4, 0, 5),
  VBUS_OVER_VOLTAGE = detail::find_register_bit(4, 0, 4),
  VBUS_INSERTED = detail::find_register_bit(4, 0, 3),
  VBUS_REMOVED = detail::find_register_bit(4, 0, 2),
  VBUS_LOW = detail::find_register_bit(4, 0, 1),
  // reg RegisterLocations::IRQ_ENABLE_REGISTER2 - 9.11.54
  BATTERY_INSERTED = detail::find_register_bit(4, 1, 7),
  BATTERY_REMOVED = detail::find_register_bit(4, 1, 6),
  BATTERY_ACTIVATED = detail::find_register_bit(4, 1, 5),
  BATTERY_DEACTIVATED = detail::find_register_bit(4, 1, 4),
  CHARGING_STARTED = detail::find_register_bit(4, 1, 3),
  CHARGING_FINISHED = detail::find_register_bit(4, 1, 2),
  BATTERY_OVER_TEMP = detail::find_register_bit(4, 1, 1),
  BATTERY_UNDER_TEMP = detail::find_register_bit(4, 1, 0),
  // reg 42 - 9.11.55
  AXP_OVER_TEMP = detail::find_register_bit(4, 2, 7),
  LOW_CHARGE_CURRENT = detail::find_register_bit(4, 2, 6),
  DCDC1_UNDER_VOLTAGE = detail::find_register_bit(4, 2, 5),
  DCDC2_UNDER_VOLTAGE = detail::find_register_bit(4, 2, 4),
  DCDC3_UNDER_VOLTAGE = detail::find_register_bit(4, 2, 3),
  SHORT_KEY_PRESS = detail::find_register_bit(4, 2, 1),
  LONG_KEY_PRESS = detail::find_register_bit(4, 2, 0),
  // reg 43 - 9.11.56
  POWERON_NOE = detail::find_register_bit(4, 3, 7),
  POWEROFF_NOE = detail::find_register_bit(4, 3, 6),
  VBUS_VALID = detail::find_register_bit(4, 3, 5),
  VBUS_INVALID = detail::find_register_bit(4, 3, 4),
  VBUS_SESSION_AB = detail::find_register_bit(4, 3, 3),
  VBUS_SESSION_END = detail::find_register_bit(4, 3, 1),
  APS_UNDER_VOLTAGE = detail::find_register_bit(4, 3, 0)
};

enum class SensorType : uint8_t {
  ACIN_VOLTAGE,
  ACIN_CURRENT,
  VBUS_VOLTAGE,
  VBUS_CURRENT,
  AXP_TEMP,
  BATTERY_TEMP,
  BATTERY_POWER,
  BATTERY_VOLTAGE,
  BATTERY_CHARGE_CURRENT,
  BATTERY_DISCHARGE_CURRENT,
  APS_VOLTAGE,
  GPIO0_VOLTAGE,
  GPIO1_VOLTAGE,
  GPIO2_VOLTAGE,
  GPIO3_VOLTAGE
};

enum class VoffVoltage : uint8_t {
  VOFF_2600MV = 0b0000,
  VOFF_2700MV = 0b0001,
  VOFF_2800MV = 0b0010,
  VOFF_2900MV = 0b0011,
  VOFF_3000MV = 0b0100,
  VOFF_3100MV = 0b0101,
  VOFF_3200MV = 0b0110,
  VOFF_3300MV = 0b0111
};

enum class ChargeCurrent : uint8_t {
  CHARGE_100MA = 0b0000,
  CHARGE_190MA = 0b0001,
  CHARGE_280MA = 0b0010,
  CHARGE_360MA = 0b0011,
  CHARGE_450MA = 0b0100,
  CHARGE_550MA = 0b0101,
  CHARGE_630MA = 0b0110,
  CHARGE_700MA = 0b0111
};

enum class ChargeVoltage : uint8_t {
  CHARGE_4100MV = 0b00000000,
  CHARGE_4150MV = 0b00100000,
  CHARGE_4200MV = 0b01000000,
  CHARGE_4360MV = 0b01100000
};

enum class VBusHoldVoltageLimit : uint8_t {
  HOLD_4000MV = 0b00000000,
  HOLD_4100MV = 0b00001000,
  HOLD_4200MV = 0b00010000,
  HOLD_4300MV = 0b00011000,
  HOLD_4400MV = 0b00100000,
  HOLD_4500MV = 0b00101000,
  HOLD_4600MV = 0b00110000,
  HOLD_4700MV = 0b00111000
};

enum class VBusHoldVoltageLimited : uint8_t { VOLTAGE_NOT_LIMITED = 0b00000000, VOLTAGE_LIMITED = 0b01000000 };

enum class VBusIpsout : uint8_t { IPSOUT_NOT_MANAGED = 0b00000000, IPSOUT_MANAGED = 0b10000000 };

enum class VBusHoldCurrentLimited : uint8_t { CURRENT_NOT_LIMITED = 0b00000000, CURRENT_LIMITED = 0b00000010 };

enum class VBusHoldCurrentLimit : uint8_t { CURRENT_LIMIT_500MA = 0b0000, CURRENT_LIMIT_100MA = 0b0001 };

enum class LDOio0Control : uint8_t {
  NMOS_OPENDRAIN = 0b000,
  GENERAL_INPUT = 0b001,
  LOWNOISE_LDO = 0b010,
  ADC_IN = 0b100,
  LOW_OUTPUT = 0b101,
  FLOATING = 0b111
};

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
class Axp192Component : public i2c::I2CDevice, public PollingComponent {
 public:
  void register_monitor(MonitorType type, Axp192BinarySensor *monitor);
  void register_irq(IrqType type, Axp192BinarySensor *irq);
  void register_sensor(SensorType type, Axp192Sensor *sensor);
  void register_output(OutputPin pin, Axp192Output *output);
  void register_switch(OutputPin pin, Axp192Switch *output);

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;
  void loop() override;

  void set_brightness(uint8_t brightness);
  uint8_t get_brightness();

  void power_off();
  void prepare_sleep();
  void configure_battery();
  bool configure_axp();

  void set_voff(VoffVoltage voff);
  void set_charge_voltage(ChargeVoltage voltage);
  void set_charge_current(ChargeCurrent current);
  void set_vbus_ipsout(VBusIpsout val);
  void set_vbus_hold_current_limited(VBusHoldCurrentLimited val);
  void set_vbus_hold_current_limit(VBusHoldCurrentLimit val);
  void set_vbus_hold_voltage_limited(VBusHoldVoltageLimited val);
  void set_vbus_hold_voltage_limit(VBusHoldVoltageLimit val);
  void set_disable_rtc(bool disable_rtc);
  void set_disable_ldo2(bool disable_ldo2);
  void set_disable_ldo3(bool disable_ldo3);
  void set_disable_dcdc1(bool disable_dcdc1);
  void set_disable_dcdc2(bool disable_dcdc2);
  void set_disable_dcdc3(bool disable_dcdc3);
  void set_dcdc1_voltage(uint32_t dcdc1_voltage);
  void set_dcdc2_voltage(uint32_t dcdc2_voltage);
  void set_dcdc3_voltage(uint32_t dcdc3_voltage);
  void set_ldo2_voltage(uint32_t ldo2_voltage);
  void set_ldo3_voltage(uint32_t ldo3_voltage);
  void set_ldoio0_voltage(uint32_t ldoio0_voltage);
  void set_ldoio0_mode(LDOio0Control mode);

  bool configure_ldo2(bool enable);
  bool configure_ldo3(bool enable);
  bool configure_dcdc1(bool enable);
  bool configure_dcdc2(bool enable);
  bool configure_dcdc3(bool enable);
  bool configure_ldoio0(bool enable);
  bool configure_ldo2_voltage(float level);
  bool configure_ldo3_voltage(float level);
  bool configure_dcdc1_voltage(float level);
  bool configure_dcdc3_voltage(float level);
  bool configure_ldoio0_voltage(float level);
  bool get_ldo2_enabled();
  bool get_ldo3_enabled();
  bool get_dcdc1_enabled();
  bool get_dcdc3_enabled();
  bool get_ldoio0_enabled();
  float get_ldo2_voltage();
  float get_ldo3_voltage();
  float get_dcdc1_voltage();
  float get_dcdc3_voltage();
  float get_ldoio0_voltage();

  bool update_register(RegisterLocations reg, uint8_t value, uint8_t clear_mask);
  bool load_register(RegisterLocations reg);
  bool save_register(RegisterLocations reg);

#ifdef USE_BINARY_SENSOR
  void enable_irq(IrqType irq);
#endif

  void update_powercontrol(OutputPin pin, bool value);

 protected:
#ifdef USE_SENSOR
  std::map<SensorType, Axp192Sensor *> sensors_{};
#endif

#ifdef USE_BINARY_SENSOR
  std::map<MonitorType, Axp192BinarySensor *> monitors_{};
  std::map<IrqType, Axp192BinarySensor *> irqs_{};
#endif

#ifdef USE_OUTPUT
  std::map<OutputPin, Axp192Output *> output_control_{};
#endif

#ifdef USE_SWITCH
  std::map<OutputPin, Axp192Switch *> power_control_{};
#endif

  std::map<RegisterLocations, uint8_t> registers_{
      {RegisterLocations::EXTEN_DCDC2_CONTROL, 0x00},              // 9.11.9 (DC-DC2 and external)
      {RegisterLocations::DCDC13_LDO23_CONTROL, 0xFF},             // 9.11.10 (DC-DC1/3 and LDO2/3 control)
      {RegisterLocations::DCDC2_VOLTAGE, 0x16},                    // 9.11.11 (DC-DC2 voltage)
      {RegisterLocations::DCDC1_VOLTAGE, 0x68},                    // 9.11.13 (DC-DC1) voltage
      {RegisterLocations::DCDC3_VOLTAGE, 0x48},                    // 9.11.14 (DC-DC3) voltage
      {RegisterLocations::LDO23_VOLTAGE, 0xCF},                    // 9.11.15 (LDO2 LDO3) voltage
      {RegisterLocations::VBUS_IPSOUT_ACCESS, 0x6F},               // 9.11.16 (vbus hold) voltage
      {RegisterLocations::VOFF_VOLTAGE, 0xF3},                     // 9.11.17 (power off) voltage
      {RegisterLocations::POWEROFF_BATTERY_CHLED_CONTROL, 0x46},   // 9.11.18 (battery control)
      {RegisterLocations::CHARGE_CONTROL_REG1, 0xC8},              // 9.11.19 (charge control)
      {RegisterLocations::BATTERY_BACKUP_CONTROL, 0x22},           // 9.11.21 (RTC)
      {RegisterLocations::PEK_PARAMETERS, 0x5D},                   // 9.11.22 (powerkey)
      {RegisterLocations::BATTERY_CHARGE_OVERTEMP_VALUE, 0x1F},    // 9.11.25 (temperature protection)
      {RegisterLocations::IRQ_ENABLE_REGISTER1, 0xD8},             // 9.11.53 (IRQ1 enable)
      {RegisterLocations::IRQ_ENABLE_REGISTER2, 0xFF},             // 9.11.53 (IRQ1 enable)
      {RegisterLocations::IRQ_ENABLE_REGISTER3, 0x3B},             // 9.11.53 (IRQ1 enable)
      {RegisterLocations::IRQ_ENABLE_REGISTER4, 0xC1},             // 9.11.53 (IRQ1 enable)
      {RegisterLocations::ADC_ENABLE_REGISTER1, 0xFF},             // 9.11.31 (ADC1 control)
      {RegisterLocations::ADC_ENABLE_REGISTER2, 0xFF},             // 9.11.32 (ADC2 control)
      {RegisterLocations::ADC_SAMPLE_FREQUENCY_TS_CONTROL, 0x33},  // 9.11.33 (ADC1 params)
      {RegisterLocations::GPIO_30_INPUT_RANGE, 0x00},              // 9.11.34 (ADC2 params)
      {RegisterLocations::GPIO_CONTROL, 0x07},                     // 9.11.30 (GPIO0 function) LDio0 enable
      {RegisterLocations::GPIO_LDO_VOLTAGE, 0xA0},                 // 9.11.39 (LDio0) voltage
  };

  std::unordered_map<RegisterLocations, uint8_t> register_masks_{
      {RegisterLocations::DCDC13_LDO23_CONTROL, 0b11110000},
      {RegisterLocations::EXTEN_DCDC2_CONTROL, 0b11111010},
      {RegisterLocations::DCDC2_VOLTAGE, 0b11000000},
      {RegisterLocations::DCDC1_VOLTAGE, 0b10000000},
      {RegisterLocations::DCDC3_VOLTAGE, 0b10000000},
      {RegisterLocations::VOFF_VOLTAGE, 0b11111000},
      {RegisterLocations::POWEROFF_BATTERY_CHLED_CONTROL, 0b00000100},
      {RegisterLocations::BATTERY_BACKUP_CONTROL, 0b00011100},
      {RegisterLocations::IRQ_ENABLE_REGISTER1, 0b00000001},
      {RegisterLocations::IRQ_ENABLE_REGISTER3, 0b00000100},
      {RegisterLocations::IRQ_ENABLE_REGISTER4, 0b00000010},
      {RegisterLocations::ADC_ENABLE_REGISTER2, 0b01111000},
      {RegisterLocations::ADC_SAMPLE_FREQUENCY_TS_CONTROL, 0b00001000},
      {RegisterLocations::GPIO_30_INPUT_RANGE, 0b11110000},
      {RegisterLocations::GPIO_CONTROL, 0b11110000},
      {RegisterLocations::GPIO_LDO_VOLTAGE, 0b00001111},
  };

 private:
  void publish_helper_(SensorType type, float state);
  void debug_log_register_(RegisterLocations reg);

  void publish_helper_(IrqType type, bool state);
  void publish_helper_(MonitorType type, bool state);
#ifdef USE_BINARY_SENSOR
  void do_irqs_();
  uint32_t last_irq_buffer_ = 0xFF;
#endif

};

}  // namespace axp192
}  // namespace esphome
