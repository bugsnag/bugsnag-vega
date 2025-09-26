#define DOCTEST_CONFIG_IMPLEMENT
#include "BugsnagNativeUT.h"
#include "../../../../packages/vega-native/kepler/turbo-modules/BugsnagFileIO.h"
#include "Kepler/turbomodule/TMLog.h"
#include <thread>


using namespace com::amazon::kepler::turbomodule;

namespace bugsnag {

// TODO REMOVE factorial tests - placeholder for unit testing
int factorial(int number) {
  return number > 1 ? factorial(number - 1) * number : 1;
}

BugsnagNativeUT::BugsnagNativeUT() {}
BugsnagNativeUT::~BugsnagNativeUT() noexcept {};

void BugsnagNativeUT::configure(std::string path) {
  // TODO REMOVE FILE IO usage - placeholder for testing the linking
  auto item = BugsnagFileIO();
  item.sha1("akakka");

  this->utOutput.open(path);
  this->context.setOption("no-breaks", true);
  this->context.setCout(&(this->utOutput));
}

double BugsnagNativeUT::runUnitTests() {
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

void BugsnagNativeUT::readOnlyMemoryCrash() {
  TMWARN("[bugsnag] triggered read only memory crash");
  // Write to a read-only page
  volatile char *ptr = (char *)somefakefunc;
  *ptr = 0;
}

int *global_ptr = nullptr;

void BugsnagNativeUT::nullptrCrash() {
  TMWARN("[bugsnag] triggered nullptr dereference crash");
  *global_ptr = 1;
}

void BugsnagNativeUT::manualAbortCrash() {
  TMWARN("[bugsnag] triggered manual abort crash");
  abort();
}

void helper_func_exception() { throw std::runtime_error("my msg"); }

void BugsnagNativeUT::throwExceptionCrash() {
  TMWARN("[bugsnag] triggered unhandled exception crash");

  std::thread exception_thread(helper_func_exception);
  exception_thread.join();
}

} // namespace bugsnag
