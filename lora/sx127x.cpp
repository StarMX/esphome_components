#include "sx127x.h"

namespace esphome {
namespace lora {

static const char *const TAG = "sx127x";

// registers
#define REG_FIFO 0x00
#define REG_OP_MODE 0x01
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08
#define REG_PA_CONFIG 0x09
#define REG_OCP 0x0b
#define REG_LNA 0x0c
#define REG_FIFO_ADDR_PTR 0x0d
#define REG_FIFO_TX_BASE_ADDR 0x0e
#define REG_FIFO_RX_BASE_ADDR 0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS 0x12
#define REG_RX_NB_BYTES 0x13
#define REG_PKT_SNR_VALUE 0x19
#define REG_PKT_RSSI_VALUE 0x1a
#define REG_RSSI_VALUE 0x1b
#define REG_MODEM_CONFIG_1 0x1d
#define REG_MODEM_CONFIG_2 0x1e
#define REG_PREAMBLE_MSB 0x20
#define REG_PREAMBLE_LSB 0x21
#define REG_PAYLOAD_LENGTH 0x22
#define REG_MODEM_CONFIG_3 0x26
#define REG_FREQ_ERROR_MSB 0x28
#define REG_FREQ_ERROR_MID 0x29
#define REG_FREQ_ERROR_LSB 0x2a
#define REG_RSSI_WIDEBAND 0x2c
#define REG_DETECTION_OPTIMIZE 0x31
#define REG_INVERTIQ 0x33
#define REG_DETECTION_THRESHOLD 0x37
#define REG_SYNC_WORD 0x39
#define REG_INVERTIQ2 0x3b
#define REG_DIO_MAPPING_1 0x40
#define REG_VERSION 0x42
#define REG_PA_DAC 0x4d

// modes
#define MODE_LONG_RANGE_MODE 0x80
#define MODE_SLEEP 0x00
#define MODE_STDBY 0x01
#define MODE_TX 0x03
#define MODE_RX_CONTINUOUS 0x05
#define MODE_RX_SINGLE 0x06

// PA config
#define PA_BOOST 0x80

// IRQ masks
#define IRQ_TX_DONE_MASK 0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK 0x40

#define RF_MID_BAND_THRESHOLD 525E6
#define RSSI_OFFSET_HF_PORT 157
#define RSSI_OFFSET_LF_PORT 164

void SX127x::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SX127x Start");

  if (this->rst_pin_ != nullptr) {
    this->rst_pin_->setup();
    this->rst_pin_->digital_write(false);
    delay(1);
    this->rst_pin_->digital_write(true);
    delay(10);
  }

  if (this->dio0_pin_ != nullptr) {
    this->dio0_pin_->setup();
    this->dio0_pin_->pin_mode(gpio::FLAG_INPUT);
  }

  // start spi
  this->spi_setup();

  if (this->read_register_(REG_VERSION) != 0x12) {
    this->mark_failed();
    return;
  }

  this->sleep();

  this->setFrequency();

  this->write_register_(REG_FIFO_TX_BASE_ADDR, 0);
  this->write_register_(REG_FIFO_RX_BASE_ADDR, 0);

  // set LNA boost
  this->write_register_(REG_LNA, this->read_register_(REG_LNA) | 0x03);

  // set auto AGC
  this->write_register_(REG_MODEM_CONFIG_3, 0x04);

  //   // set output power to 17 dBm
  this->setTxPower();

  this->setBandwidth();

  this->setSyncWord(0x12);

  this->setPreambleLength();

  this->setSpreadingFactor();
  this->setCodingRate4();
  // enable Crc
  this->write_register_(REG_MODEM_CONFIG_2, this->read_register_(REG_MODEM_CONFIG_2) | 0x04);

  // put in standby mode
  this->idle();
  ESP_LOGCONFIG(TAG, "Setting up SX127x Done");
}

void SX127x::update() { ESP_LOGD(TAG, "Lora RSSI %d", this->rssi()); }

void SX127x::loop() {
  this->receive();
  if (this->received()) {
    uint8_t len = this->available();
    ESP_LOGD(TAG, "Lora message lenght %d", len);
  }
}

void SX127x::sleep() { this->write_register_(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP); }
void SX127x::idle() { this->write_register_(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY); }

void SX127x::setSyncWord(int sw) { this->write_register_(REG_SYNC_WORD, sw); }

void SX127x::setCodingRate4() {
  int cr = this->coding_rate_ - 4;
  this->write_register_(REG_MODEM_CONFIG_1, (this->read_register_(REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void SX127x::setSpreadingFactor() {
  if (this->spreading_factor_ == 6) {
    this->write_register_(REG_DETECTION_OPTIMIZE, 0xc5);
    this->write_register_(REG_DETECTION_THRESHOLD, 0x0c);
  } else {
    this->write_register_(REG_DETECTION_OPTIMIZE, 0xc3);
    this->write_register_(REG_DETECTION_THRESHOLD, 0x0a);
  }
  this->write_register_(REG_MODEM_CONFIG_2,
                        (this->read_register_(REG_MODEM_CONFIG_2) & 0x0f) | ((this->spreading_factor_ << 4) & 0xf0));
  // setLdoFlag();
}

void SX127x::setPreambleLength() {
  this->write_register_(REG_PREAMBLE_MSB, (uint8_t) (this->preamble_length_ >> 8));
  this->write_register_(REG_PREAMBLE_LSB, (uint8_t) (this->preamble_length_ >> 0));
}

void SX127x::setTxPower() {
  // RF9x module uses PA_BOOST pin
  if (this->tx_power_ < 2)
    this->tx_power_ = 2;
  else if (this->tx_power_ > 17)
    this->tx_power_ = 17;
  this->write_register_(REG_PA_CONFIG, PA_BOOST | (this->tx_power_ - 2));
}

void SX127x::setBandwidth() {
  if (bandwidth_ < 10) {
    this->write_register_(REG_MODEM_CONFIG_1, (this->read_register_(REG_MODEM_CONFIG_1) & 0x0f) | (bandwidth_ << 4));
  }
  // setLdoFlag();
}
void SX127x::setFrequency() {
  uint64_t frf = ((uint64_t) this->frequency_ << 19) / 32000000;
  this->write_register_(REG_FRF_MSB, (uint8_t) (frf >> 16));
  this->write_register_(REG_FRF_MID, (uint8_t) (frf >> 8));
  this->write_register_(REG_FRF_LSB, (uint8_t) (frf >> 0));
}

int8_t SX127x::rssi() {
  return (this->read_register_(REG_RSSI_VALUE) -
          (this->frequency_ < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

int8_t SX127x::available() { return this->read_register_(REG_RX_NB_BYTES); }

void SX127x::receive() {
  this->write_register_(REG_DIO_MAPPING_1, 0x00);  // DIO0 => RXDONE

  this->write_register_(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

bool SX127x::received(void) { return (this->read_register_(REG_IRQ_FLAGS) & IRQ_RX_DONE_MASK) ? 1 : 0; }

// void SX127x::send_packet(uint8_t *buf, int size) {
//   /*
//    * Transfer data to radio.
//    */
//   this->idle();
//   this->write_register_(REG_FIFO_ADDR_PTR, 0);

// #if BUFFER_IO
//   lora_write_reg_buffer(REG_FIFO, buf, size);
// #else
//   for (int i = 0; i < size; i++)
//     this->write_register_(REG_FIFO, *buf++);
// #endif

//   this->write_register_(REG_PAYLOAD_LENGTH, size);

//   /*
//    * Start transmission and wait for conclusion.
//    */
//   this->write_register_(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
//   int loop = 0;
//   while (1) {
//     int irq = this->read_register_(REG_IRQ_FLAGS);
//     ESP_LOGD(TAG, "lora_read_reg=0x%x", irq);
//     if ((irq & IRQ_TX_DONE_MASK) == IRQ_TX_DONE_MASK)
//       break;
//     loop++;
//     if (loop == 10)
//       break;
//     vTaskDelay(2);
//   }
//   if (loop == 10) {
//     __send_packet_lost++;
//     ESP_LOGE(TAG, "lora_send_packet Fail");
//   }
//   this->write_register_(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
// }

void SX127x::read_register_(uint8_t reg, uint8_t *buffer, size_t length) {
  this->enable();
  this->transfer_byte(reg);
  this->read_array(buffer, length);
  this->disable();
}

uint8_t SX127x::read_register_(uint8_t reg) {
  this->enable();
  this->transfer_byte(reg);
  uint8_t value = this->read_byte();
  this->disable();
  return value;
}

void SX127x::write_register_(uint8_t reg, uint8_t *value, size_t length) {
  this->enable();
  this->transfer_byte(reg | 0x80);
  this->transfer_array(value, length);
  this->disable();
}
void SX127x::write_register_(uint8_t reg, uint8_t value) {
  uint8_t arr[1] = {value};
  this->write_register_(reg, arr, 1);
}

void SX127x::dump_config() {
  ESP_LOGCONFIG(TAG, "SX127x:");
  LOG_PIN("  CS Pin: ", this->cs_);
  LOG_PIN("  RST Pin: ", this->rst_pin_);
  LOG_PIN("  DIO0 Pin: ", this->dio0_pin_);
  ESP_LOGCONFIG(TAG, "  frequency: %.2f MHz", (float) this->frequency_ / 1000000);
  ESP_LOGCONFIG(TAG, "  bandwidth: %d", this->bandwidth_);
  ESP_LOGCONFIG(TAG, "  tx power: %d", this->tx_power_);
  ESP_LOGCONFIG(TAG, "  preamble length: %d", this->preamble_length_);
  ESP_LOGCONFIG(TAG, "  spreading factor: %d", this->spreading_factor_);
  ESP_LOGCONFIG(TAG, "  coding rate: %d", this->coding_rate_);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Configuring SX127x failed");
  }
};

}  // namespace lora
}  // namespace esphome