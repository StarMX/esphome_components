#pragma once
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome {
namespace templates {

class TemplatesComponent : public PollingComponent{
  public:
    void setup() override;
    void dump_config() override;
    float get_setup_priority() const override;
    void update() override;
    void loop() override;

    void add_on_data_received_callback(std::function<void(std::string)> callback) {
      this->data_received_callback_.add(std::move(callback));
    }

    void send(const std::string &message);

  protected:
    std::string message_;
  protected:
    CallbackManager<void(std::string)> data_received_callback_;
};

class TemplatesDataReceivedMessageTrigger : public Trigger<std::string> {
 public:
  explicit TemplatesDataReceivedMessageTrigger(TemplatesComponent *parent) {
    parent->add_on_data_received_callback([this](const std::string &message) { this->trigger(message); });
  }
};


template<typename... Ts> class TemplatesSendAction : public Action<Ts...> {
 public:
  TemplatesSendAction(TemplatesComponent *parent) : parent_(parent) {}
  TEMPLATABLE_VALUE(std::string, message)

  void play(Ts... x) {
    auto message = this->message_.value(x...);
    this->parent_->send(message);
  }

 protected:
  TemplatesComponent *parent_;
};


}
}