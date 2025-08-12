#include "generated/BugsnagNativeUTSpec.h"
#include "doctest.h"

#include <fstream>

namespace bugsnag {

class BugsnagNativeUT : public BugsnagNativeUTSpec {
public:
  BugsnagNativeUT();
  ~BugsnagNativeUT() noexcept;

  void configure(std::string path) override;
  double runUnitTests() override;
  void readOnlyMemoryCrash() override;
  void nullptrCrash() override;
  void manualAbortCrash() override;
  void throwExceptionCrash() override;

private:
  doctest::Context context;
  std::ofstream utOutput;
};

} // namespace bugsnag
