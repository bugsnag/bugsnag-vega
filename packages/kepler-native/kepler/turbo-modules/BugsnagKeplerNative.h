#ifndef BUGSNAG_KEPLER_NATIVE_H
#define BUGSNAG_KEPLER_NATIVE_H

#include "BugsnagClient.h"
#include "Kepler/turbomodule/KeplerTurboModule.h"

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
class BugsnagKeplerNative : public TM_API_NAMESPACE::KeplerTurboModule {
public:
  BugsnagKeplerNative();

  void aggregateMethods(
      TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>
          &methodAggregator) const noexcept;

  utils::json::JsonContainer configure(utils::json::JsonContainer config);
  void mark_launch_completed();
  std::string generate_uuid();
  std::string get_device_id();
  void set_device_id(std::string device_id);
  void native_crash();

  void leave_breadcrumb(utils::json::JsonContainer crumb);
  void set_metadata(std::string metadata_str);
  void clear_metadata();
  void set_features(std::string features_str);
  void clear_features();

private:
  Client *bugsnag;
  std::string device_id;
};
} // namespace bugsnag

#endif // BUGSNAG_KEPLER_NATIVE_H
