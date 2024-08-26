#pragma once

#include "esphome/components/esp32_ble_server/ble_characteristic.h"
#include "esphome/components/esp32_ble_server/ble_server.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/preferences.h"

#ifdef USE_ESP32
namespace esphome {
namespace ble_server_template {

using namespace esp32_ble_server;

class TemplateBleServer : public Component, public BLEServiceComponent {
 public:
 
  TemplateBleServer();
  void dump_config() override;
  void loop() override;
  void setup() override;
  void setup_characteristics();
  void on_client_disconnect() override;
  void on_client_connect() override;
  
  float get_setup_priority() const override;
  void start() override;
  void stop() override;



 protected:
  enum State : uint8_t {
    STATE_STOPPED = 0x00,
    STATE_RUNNING = 0x01,
  };

  BLEService *service_;
  BLECharacteristic *my_characteristic_;

  State state_{STATE_STOPPED};

  void set_state_(State state);
  void send_response_(std::vector<uint8_t> &response);
};

}  // namespace ble_server_template
}  // namespace esphome

#endif
