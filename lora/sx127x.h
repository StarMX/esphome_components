#pragma once

#include "esphome/components/spi/spi.h"

namespace esphome {
namespace lora {

#if ESPHOME_VERSION_CODE < VERSION_CODE(2023, 12, 0)
/**
 * Store interrupt related information.
 */
struct Store {
  volatile bool have;
  ISRInternalGPIOPin pin;

  static void gpio_intr(Store *store);
};
#endif  // VERSION_CODE(2023, 12, 0)

class SX127x : public PollingComponent,
               public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW, spi::CLOCK_PHASE_LEADING,
                                     spi::DATA_RATE_10MHZ> {
 public:
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;
  void set_reset_pin(InternalGPIOPin *rst_pin) { this->rst_pin_ = rst_pin; }
  void set_dio0_pin(InternalGPIOPin *dio0_pin) { this->dio0_pin_ = dio0_pin; }
  void set_frequency(long frequency) { this->frequency_ = frequency; }
  void set_bandwidth(uint8_t bandwidth) { this->bandwidth_ = bandwidth; }
  void set_tx_power(uint8_t power) { this->tx_power_ = power; }
  void set_preamble_length(uint8_t preamble_length) { this->preamble_length_ = preamble_length; }
  void set_spreading_factor(uint8_t spreading_factor) { this->spreading_factor_ = spreading_factor; }
  void set_coding_rate(uint8_t coding_rate) { this->coding_rate_ = coding_rate; }

  int8_t rssi();
  void receive();
  int8_t available();
  bool receivePacket(uint8_t *buf, uint8_t size);
  void sendPacket(uint8_t *buf, uint8_t size, bool async = true);

  void add_on_data_received_callback(std::function<void(const char *, uint8_t)> callback) {
    this->data_received_callback_.add(std::move(callback));
  }
  void set_send_lora_data(const std::string data) {
#ifdef CONFIG_LORA_GATEWAY
    this->enableInvertIQ();
#else
    this->disableInvertIQ();
#endif
    this->sendPacket((uint8_t *) data.data(), data.size());
#ifdef CONFIG_LORA_GATEWAY
    this->disableInvertIQ();
#else
    this->enableInvertIQ();
#endif
  }

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
  void enableInvertIQ();
  void disableInvertIQ();
  void explicitHeaderMode();
  bool isTransmitting();

 protected:
#if ESPHOME_VERSION_CODE < VERSION_CODE(2023, 12, 0)
  Store store_;
#endif

  void write_register_(uint8_t address, uint8_t value);
  void write_register_(uint8_t reg, uint8_t *value, size_t length);
  uint8_t read_register_(uint8_t address);
  void read_register_(uint8_t reg, uint8_t *buffer, size_t length);
  InternalGPIOPin *rst_pin_{nullptr};
  InternalGPIOPin *dio0_pin_{nullptr};
  long frequency_;
  uint8_t bandwidth_;
  uint8_t tx_power_;
  uint8_t preamble_length_;
  uint8_t spreading_factor_;
  uint8_t coding_rate_;
  CallbackManager<void(const char *, uint8_t)> data_received_callback_;
};

class LoraDataReceivedMessageTrigger : public Trigger<const char *, uint8_t> {
 public:
  explicit LoraDataReceivedMessageTrigger(SX127x *parent) {
    parent->add_on_data_received_callback([this](const char *data, uint8_t length) { this->trigger(data, length); });
  }
};

template<typename... Ts> class LoraSendAction : public Action<Ts...> {
 public:
  LoraSendAction(SX127x *parent) : parent_(parent) {}
  TEMPLATABLE_VALUE(std::string, message)

  void play(Ts... x) {
    auto message = this->message_.value(x...);
    // parent_->set_send_lora_data(message);
    parent_->sendPacket((uint8_t *) message.data(), message.size());
  }

 protected:
  SX127x *parent_;
};

}  // namespace lora
}  // namespace esphome