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
  void markLaunchCompleted();
  std::string generateUUID();
  std::string getDeviceID();
  void setDeviceID(std::string device_id);
  void nativeCrash();

  void leaveBreadcrumb(utils::json::JsonContainer crumb);
  void addMetadata(std::string metadata_str);
  void clearMetadata();
  void addFeatures(std::string features_str);
  void clearFeatures();

private:
  Client *bugsnag;
  std::string device_id;
};
} // namespace bugsnag

#endif // BUGSNAG_KEPLER_NATIVE_H
