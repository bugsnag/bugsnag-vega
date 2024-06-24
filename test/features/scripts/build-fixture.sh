#!/usr/bin/env bash
set -e

if [[ "$AMZN_AUTHENTICATION" == "" ]]; then
  echo "Missing required env var AMZN_AUTHENTICATION"
  exit 1
fi

projectRoot="`dirname $0`/../../.."
rm -rf "$projectRoot/.verdaccio/@bugsnag"
pushd "$projectRoot"

# start the verdaccio server
npm run local-npm:start &

# build and publish the latest packages
./scripts/publish-local.sh

# Install packages into the test fixture
popd
fixtureDir="`dirname $0`/../fixtures/keplertestapp"
pushd "$fixtureDir"
npm run clean
rm -rf build package-lock.json
npm i --force --registry 'http://0.0.0.0:5539'

# build the test fixture
kepler build

# stop the verdaccio server
lsof -t -i:5539 | xargs kill -9
