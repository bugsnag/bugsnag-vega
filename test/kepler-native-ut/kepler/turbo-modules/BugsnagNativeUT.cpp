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
  methodAggregator.addMethod("readOnlyMemoryCrash", 0,
                             &BugsnagNativeUT::read_only_memory_crash);
  methodAggregator.addMethod("nullptrCrash", 0,
                             &BugsnagNativeUT::nullptr_dereference_crash);
  methodAggregator.addMethod("manualAbortCrash", 0,
                             &BugsnagNativeUT::manual_abort_crash);
  methodAggregator.addMethod("throwExceptionCrash", 0,
                             &BugsnagNativeUT::throw_exception_crash);
}

void BugsnagNativeUT::configure(std::string path) {
  // TODO REMOVE FILE IO usage - placeholder for testing the linking
  auto item = BugsnagFileIO();
  item.sha1("akakka");

  this->utOutput.open(path);
  this->context.setOption("no-breaks", true);
  this->context.setCout(&(this->utOutput));
}

int BugsnagNativeUT::runUnitTests() {
  int res = context.run();
  this->utOutput.close();
  return res;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial(1) == 1);
  CHECK(factorial(2) == 2);
  CHECK(factorial(3) == 6);
  CHECK(factorial(10) == 3628800);
}

static void __attribute__((used)) somefakefunc(void) {}

void BugsnagNativeUT::read_only_memory_crash() {
  // Write to a read-only page
  volatile char *ptr = (char *)somefakefunc;
  *ptr = 0;
}

void BugsnagNativeUT::nullptr_dereference_crash() {
  int *p = nullptr;
  *p = 1;
}

void BugsnagNativeUT::manual_abort_crash() { abort(); }

void BugsnagNativeUT::throw_exception_crash() { throw std::bad_exception(); }

} // namespace bugsnag