#include "BugsnagClient.h"
#include "generated/BugsnagKeplerNativeSpec.h"

namespace bugsnag {

class BugsnagKeplerNative : public BugsnagKeplerNativeSpec {
public:
  BugsnagKeplerNative();
  ~BugsnagKeplerNative() noexcept;

  com::amazon::kepler::turbomodule::JSObject configure(com::amazon::kepler::turbomodule::JSObject configuration) override;
  void markLaunchCompleted() override;

  std::string generateUUID() override;
  std::string getDeviceID() override;
  void setDeviceID(std::string id) override;
  void setDevice(com::amazon::kepler::turbomodule::JSObject deviceInfo) override;

  void leaveBreadcrumb(com::amazon::kepler::turbomodule::JSObject crumb) override;
  void setMetadata(std::string metadataStr) override;
  void clearMetadata() override;
  void setFeatures(std::string featuresStr) override;
  void clearFeatures() override;

  void setUser(com::amazon::kepler::turbomodule::JSObject userInfo) override;
  void clearUser() override;

  void setApp(com::amazon::kepler::turbomodule::JSObject appInfo) override;

  void nativeCrash() override;

private:
  Client *bugsnag;
  std::string device_id;
};

} // namespace bugsnag
