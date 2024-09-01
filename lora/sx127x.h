#pragma once

#include "esphome/components/spi/spi.h"

namespace esphome {
namespace lora {

class SX127x : public PollingComponent,
               public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW, spi::CLOCK_PHASE_TRAILING,
                                     spi::DATA_RATE_1MHZ> {
 public:
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;
  void set_reset_pin(InternalGPIOPin *rst_pin) { this->rst_pin_ = rst_pin; }
  void set_dio0_pin(InternalGPIOPin *dio0_pin) { this->dio0_pin_ = dio0_pin; }
  void set_frequency(uint32_t frequency) { this->frequency_ = frequency; }
  void set_bandwidth(uint8_t bandwidth) { this->bandwidth_ = bandwidth; }
  void set_tx_power(uint8_t power) { this->tx_power_ = power; }
  void set_preamble_length(uint32_t preamble_length) { this->preamble_length_ = preamble_length; }
  void set_spreading_factor(uint8_t spreading_factor) { this->spreading_factor_ = spreading_factor; }
  void set_coding_rate(uint8_t coding_rate) { this->coding_rate_ = coding_rate; }

  int8_t rssi();
  void receive();
  int8_t available();

 private:
  bool received();
  void sleep();
  void setFrequency();
  void setBandwidth();
  void setTxPower();
  void idle();
  void setSyncWord(int sw);
  void setPreambleLength();
  void setSpreadingFactor();
  void setCodingRate4();

 protected:
  void write_register_(uint8_t address, uint8_t value);
  void write_register_(uint8_t reg, uint8_t *value, size_t length);
  uint8_t read_register_(uint8_t address);
  void read_register_(uint8_t reg, uint8_t *buffer, size_t length);
  InternalGPIOPin *rst_pin_{nullptr};
  InternalGPIOPin *dio0_pin_{nullptr};
  uint32_t frequency_;
  uint8_t bandwidth_;
  uint8_t tx_power_;
  uint32_t preamble_length_;
  uint8_t spreading_factor_;
  uint8_t coding_rate_;
};

}  // namespace lora
}  // namespace esphome