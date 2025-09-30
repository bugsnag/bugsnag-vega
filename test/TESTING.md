# Kepler testing

## Prerequisites

You will need to ensure that the kepler platform tools are installed and set up on the machine.

## Building the test fixture

To build the test fixture, run `features/scripts/build-fixture.sh`. This script will:
- Start the verdaccio server
- Build and publish the latest packages
- Install packages into the test fixture
- Build the test fixture

## Running the end-to-end tests

The end-to-end tests run locally on the kepler simulator. To run the end-to-end tests, run the following from the `test/` directory:

```sh
bundle exec maze-runner
```

Or to run a single feature file:

```sh
bundle exec maze-runner features/handled.feature
```
