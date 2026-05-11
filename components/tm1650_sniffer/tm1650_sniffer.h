#pragma once

#include <array>
#include <cstdint>

#include "driver/gpio.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace tm1650_sniffer {

class TM1650Sniffer : public Component {
 public:
  void set_scl_pin(InternalGPIOPin *pin) { this->scl_pin_ = pin; }
  void set_sda_pin(InternalGPIOPin *pin) { this->sda_pin_ = pin; }
  void set_enabled(bool enabled) { this->enabled_ = enabled; }
  void set_log_raw(bool log_raw) { this->log_raw_ = log_raw; }

  void setup() override;
  void loop() override;
  void dump_config() override;

 protected:
  struct EdgeEvent {
    uint32_t micros;
    bool scl;
    bool sda;
  };

  static constexpr uint8_t EVENT_BUFFER_SIZE = 96;
  static constexpr uint8_t MAX_FRAME_BYTES = 32;

  static void gpio_isr(void *arg);

  void process_event_(const EdgeEvent &event);
  void reset_frame_();
  void finish_frame_();
  bool push_event_from_isr_(const EdgeEvent &event);
  bool pop_event_(EdgeEvent *event);
  void uninstall_interrupts_();

  InternalGPIOPin *scl_pin_{nullptr};
  InternalGPIOPin *sda_pin_{nullptr};
  gpio_num_t scl_gpio_{GPIO_NUM_NC};
  gpio_num_t sda_gpio_{GPIO_NUM_NC};

  bool enabled_{true};
  bool log_raw_{true};
  bool interrupts_installed_{false};
  bool last_scl_{true};
  bool last_sda_{true};

  volatile uint8_t event_head_{0};
  volatile uint8_t event_tail_{0};
  volatile uint32_t dropped_events_{0};
  std::array<EdgeEvent, EVENT_BUFFER_SIZE> events_{};

  bool frame_active_{false};
  uint8_t current_byte_{0};
  uint8_t bit_index_{0};
  uint8_t frame_len_{0};
  std::array<uint8_t, MAX_FRAME_BYTES> frame_{};
};

}  // namespace tm1650_sniffer
}  // namespace esphome
