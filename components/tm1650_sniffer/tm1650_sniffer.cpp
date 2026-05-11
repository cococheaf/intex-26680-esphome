#include "tm1650_sniffer.h"

#include <cinttypes>
#include <string>

#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tm1650_sniffer {

static const char *const TAG = "tm1650_sniffer";

void TM1650Sniffer::setup() {
  if (!this->enabled_) {
    ESP_LOGI(TAG, "TM1650 sniffer disabled by configuration");
    return;
  }

  if (this->scl_pin_ == nullptr || this->sda_pin_ == nullptr) {
    ESP_LOGE(TAG, "SCL and SDA pins must be configured");
    this->mark_failed();
    return;
  }

  this->scl_pin_->setup();
  this->sda_pin_->setup();

  this->scl_gpio_ = static_cast<gpio_num_t>(this->scl_pin_->get_pin());
  this->sda_gpio_ = static_cast<gpio_num_t>(this->sda_pin_->get_pin());
  this->last_scl_ = this->scl_pin_->digital_read();
  this->last_sda_ = this->sda_pin_->digital_read();

  esp_err_t err = gpio_install_isr_service(0);
  if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
    ESP_LOGE(TAG, "Failed to install GPIO ISR service: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  err = gpio_set_intr_type(this->scl_gpio_, GPIO_INTR_ANYEDGE);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to configure SCL interrupt: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  err = gpio_set_intr_type(this->sda_gpio_, GPIO_INTR_ANYEDGE);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to configure SDA interrupt: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  err = gpio_isr_handler_add(this->scl_gpio_, &TM1650Sniffer::gpio_isr, this);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to attach SCL interrupt: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  err = gpio_isr_handler_add(this->sda_gpio_, &TM1650Sniffer::gpio_isr, this);
  if (err != ESP_OK) {
    gpio_isr_handler_remove(this->scl_gpio_);
    ESP_LOGE(TAG, "Failed to attach SDA interrupt: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  this->interrupts_installed_ = true;
  ESP_LOGI(TAG, "TM1650 sniffer enabled on SCL GPIO%u and SDA GPIO%u",
           static_cast<unsigned>(this->scl_gpio_), static_cast<unsigned>(this->sda_gpio_));
}

void TM1650Sniffer::loop() {
  if (!this->enabled_ || !this->interrupts_installed_)
    return;

  EdgeEvent event;
  uint8_t processed = 0;
  while (this->pop_event_(&event) && processed < 64) {
    this->process_event_(event);
    processed++;
  }

  uint32_t dropped = this->dropped_events_;
  if (dropped > 0) {
    this->dropped_events_ = 0;
    ESP_LOGW(TAG, "Dropped %" PRIu32 " TM1650 edge events; bus may be too fast for current logging", dropped);
  }
}

void TM1650Sniffer::dump_config() {
  ESP_LOGCONFIG(TAG, "TM1650 Sniffer:");
  ESP_LOGCONFIG(TAG, "  Enabled: %s", YESNO(this->enabled_));
  LOG_PIN("  SCL Pin: ", this->scl_pin_);
  LOG_PIN("  SDA Pin: ", this->sda_pin_);
  ESP_LOGCONFIG(TAG, "  Raw frame logging: %s", YESNO(this->log_raw_));
}

void TM1650Sniffer::gpio_isr(void *arg) {
  auto *self = static_cast<TM1650Sniffer *>(arg);
  EdgeEvent event{};
  event.micros = micros();
  event.scl = gpio_get_level(self->scl_gpio_) != 0;
  event.sda = gpio_get_level(self->sda_gpio_) != 0;
  self->push_event_from_isr_(event);
}

bool TM1650Sniffer::push_event_from_isr_(const EdgeEvent &event) {
  uint8_t next = (this->event_head_ + 1) % EVENT_BUFFER_SIZE;
  if (next == this->event_tail_) {
    this->dropped_events_++;
    return false;
  }

  this->events_[this->event_head_] = event;
  this->event_head_ = next;
  return true;
}

bool TM1650Sniffer::pop_event_(EdgeEvent *event) {
  if (this->event_tail_ == this->event_head_)
    return false;

  *event = this->events_[this->event_tail_];
  this->event_tail_ = (this->event_tail_ + 1) % EVENT_BUFFER_SIZE;
  return true;
}

void TM1650Sniffer::process_event_(const EdgeEvent &event) {
  const bool scl_rising = !this->last_scl_ && event.scl;
  const bool sda_falling = this->last_sda_ && !event.sda;
  const bool sda_rising = !this->last_sda_ && event.sda;

  if (event.scl && sda_falling) {
    this->reset_frame_();
    this->frame_active_ = true;
    ESP_LOGVV(TAG, "START at %" PRIu32 " us", event.micros);
  } else if (event.scl && sda_rising) {
    if (this->frame_active_)
      this->finish_frame_();
    this->reset_frame_();
    ESP_LOGVV(TAG, "STOP at %" PRIu32 " us", event.micros);
  } else if (this->frame_active_ && scl_rising) {
    if (this->bit_index_ < 8) {
      this->current_byte_ = static_cast<uint8_t>((this->current_byte_ << 1) | (event.sda ? 1 : 0));
      this->bit_index_++;
    } else {
      if (this->frame_len_ < MAX_FRAME_BYTES) {
        this->frame_[this->frame_len_++] = this->current_byte_;
      } else {
        ESP_LOGW(TAG, "TM1650 frame exceeded %u bytes; truncating", MAX_FRAME_BYTES);
      }
      this->current_byte_ = 0;
      this->bit_index_ = 0;
    }
  }

  this->last_scl_ = event.scl;
  this->last_sda_ = event.sda;
}

void TM1650Sniffer::reset_frame_() {
  this->frame_active_ = false;
  this->current_byte_ = 0;
  this->bit_index_ = 0;
  this->frame_len_ = 0;
}

void TM1650Sniffer::finish_frame_() {
  if (this->frame_len_ == 0)
    return;

  if (!this->log_raw_)
    return;

  std::string bytes;
  bytes.reserve(this->frame_len_ * 5);
  char item[8];
  for (uint8_t i = 0; i < this->frame_len_; i++) {
    snprintf(item, sizeof(item), "0x%02X", this->frame_[i]);
    if (!bytes.empty())
      bytes += " ";
    bytes += item;
  }

  ESP_LOGI(TAG, "TM1650 frame (%u byte%s): %s", this->frame_len_, this->frame_len_ == 1 ? "" : "s",
           bytes.c_str());
}

}  // namespace tm1650_sniffer
}  // namespace esphome
