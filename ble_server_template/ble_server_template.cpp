#include "ble_server_template.h"

#include "esphome/components/esp32_ble/ble.h"
#include "esphome/components/esp32_ble_server/ble_2902.h"
#include "esphome/core/application.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace ble_server_template {

static const char *const TAG = "ble_server_template";

static const uint16_t SERVICE_UUID = 0x7899;
static const uint16_t CHARACTERISTIC_UUID = 0x0002;

TemplateBleServer::TemplateBleServer() { 

 }

void TemplateBleServer::setup() {
  global_ble_server->create_service(ESPBTUUID::from_uint16(SERVICE_UUID), true);
  this->service_ = global_ble_server->get_service(ESPBTUUID::from_uint16(SERVICE_UUID));
  this->setup_characteristics();
  global_ble_server->set_manufacturer("HAHAHAHA");
}

//add characteristics here
void TemplateBleServer::setup_characteristics() {
  this->my_characteristic_ = this->service_->create_characteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY |
                                                                 BLECharacteristic::PROPERTY_WRITE |
                                                                 BLECharacteristic::PROPERTY_WRITE_NR);
                                                                 
  this->my_characteristic_->on_write([this](const std::vector<uint8_t> &data) {
    if (!data.empty()) {
      std::string s(data.begin(), data.end());
      ESP_LOGD(TAG, s.c_str());
    }
  });
  //make a descriptor
  // BLEDescriptor *my_characteristic_descriptor = new BLE2902();
  this->my_characteristic_->add_descriptor(new BLE2902());
}

void TemplateBleServer::loop() {
  this->set_state_(global_ble_server->is_running() ? STATE_RUNNING: STATE_STOPPED);
  switch (this->state_) {
    case STATE_STOPPED:
      if (this->service_->is_created() && this->service_->is_running()) {
          this->service_->stop();
      }
      break;
    case STATE_RUNNING:
      if (this->service_->is_created() && !this->service_->is_running()) {
           this->service_->start();
          global_ble_server->get_service(ESPBTUUID::from_uint16(0x180A))->get_characteristic(ESPBTUUID::from_uint16(0x2A26))->set_value("V1.0");
           ESP_LOGD(TAG, "Service running");
      }
      break;
  }
}

void TemplateBleServer::start() {
  if (!this->service_->is_starting()) {
    ESP_LOGD(TAG, "Service running");
    this->service_->start();
  }
}

void TemplateBleServer::stop() {
  this->set_timeout("end-service", 1000, [this] {
    this->service_->stop();
    this->set_state_(STATE_STOPPED);
  });
}

//unimplimented response sending code.
void TemplateBleServer::send_response_(std::vector<uint8_t> &response) {
  this->my_characteristic_->set_value(response);
  // this->my_characteristic_->set_value(response.data(),response.size());
  this->my_characteristic_->notify();
}

float TemplateBleServer::get_setup_priority() const { return setup_priority::AFTER_BLUETOOTH; }

void TemplateBleServer::dump_config() { ESP_LOGCONFIG(TAG, "TemplateBleServer:"); }


void TemplateBleServer::set_state_(State state) {
  ESP_LOGV(TAG, "Setting state: %d", state);
  this->state_ = state;
}


void TemplateBleServer::on_client_disconnect() { 
  ESP_LOGD(TAG, "BLE Client disconnected");

 };

void TemplateBleServer::on_client_connect() { 
  ESP_LOGD(TAG, "BLE Client connected");

 };

}  // namespace ble_server_template
}  // namespace esphome

#endif
