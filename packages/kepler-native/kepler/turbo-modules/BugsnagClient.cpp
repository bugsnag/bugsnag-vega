#include "BugsnagClient.h"
#include "./utils/bsg_event.h"

namespace bugsnag {
Client *global_client;

Client::Client(std::unique_ptr<Configuration> config)
    : event_dir(config->storage_dir), is_launching(true),
      current_event(std::make_unique<Event>(config->max_breadcrumbs)) {
  this->config = std::move(config);
  this->current_event->configure(this->config->api_key,
                                 this->config->storage_dir);
}

void Client::mark_launch_completed() {
  this->is_launching = false;
  auto now = std::chrono::system_clock::now();
  this->start_time = std::chrono::system_clock::to_time_t(now);
}

void Client::leave_breadcrumb(bsg_breadcrumb_type type, std::string message,
                              std::string metadata, time_t timestamp) {
  if (this->current_event == nullptr) {
    return;
  }
  this->current_event->leave_breadcrumb(type, message, metadata, timestamp);
}

void Client::set_metadata(std::string metadata_str) {
  if (this->current_event == nullptr) {
    return;
  }
  this->current_event->set_metadata(metadata_str);
}

std::string Client::get_metadata() {
  if (this->current_event == nullptr) {
    return "";
  }
  return this->current_event->get_metadata();
}

void Client::clear_metadata() {
  if (this->current_event == nullptr) {
    return;
  }
  this->current_event->clear_metadata();
}

void Client::set_features(std::string features_str) {
  if (this->current_event == nullptr) {
    return;
  }
  this->current_event->set_features(features_str);
}

std::string Client::get_features() {
  if (this->current_event == nullptr) {
    return "";
  }
  return this->current_event->get_features();
}

void Client::clear_features() {
  if (this->current_event == nullptr) {
    return;
  }
  this->current_event->clear_features();
}

Event *Client::release_event() { return this->current_event.release(); }

time_t Client::get_app_start_time() { return this->start_time; }

bool Client::get_is_launching() { return this->is_launching.load(); }
} // namespace bugsnag
