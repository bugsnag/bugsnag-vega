#define DOCTEST_CONFIG_IMPLEMENT
#include "BugsnagNativeUT.h"
#include "../../../../packages/kepler-native/kepler/turbo-modules/BugsnagFileIO.h"
#include "Kepler/turbomodule/TMLog.h"

#define TM_API_NAMESPACE com::amazon::kepler::turbomodule

namespace bugsnag {
// TODO REMOVE factorial tests - placeholder for unit testing
int factorial(int number) {
  return number > 1 ? factorial(number - 1) * number : 1;
}

BugsnagNativeUT::BugsnagNativeUT()
    : TM_API_NAMESPACE::KeplerTurboModule("BugsnagNativeUT") {}

void BugsnagNativeUT::aggregateMethods(
    TM_API_NAMESPACE::MethodAggregator<TM_API_NAMESPACE::KeplerTurboModule>
        &methodAggregator) const noexcept {
  methodAggregator.addMethod("configure", 1, &BugsnagNativeUT::configure);
  methodAggregator.addMethod("runUnitTests", 0, &BugsnagNativeUT::runUnitTests);
}

void BugsnagNativeUT::configure() {
  // TODO REMOVE FILE IO usage - placeholder for testing the linking
  auto item = BugsnagFileIO();
  item.sha1("akakka");
  this->context.setOption("no-breaks", true);
  this->context.setCout(&(this->utOutput));
}

int BugsnagNativeUT::runUnitTests() {
  int res = context.run();
  std::string message = "[bugsnag] Unit test output: " + this->utOutput.str();
  TMINFO(message);
  return res;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial(1) == 1);
  CHECK(factorial(2) == 2);
  CHECK(factorial(3) == 6);
  CHECK(factorial(10) == 3628800);
}
} // namespace bugsnag