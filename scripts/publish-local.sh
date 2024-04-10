#!/usr/bin/env bash
set -e

if [[ "$VERSION_IDENTIFIER" == "" ]]; then
  export VERSION_IDENTIFIER="0.0.0"
fi

projectRoot="`dirname $0`/.."

pushd "$projectRoot/packages/kepler-native"
npm run clean
rm -rf bugsnag-kepler-native-0.1.0.tgz CMakeUserPresets.json build
kepler build
popd
npm run local-npm:publish

pushd "$projectRoot/packages/kepler-native-ut"
npm run clean
rm -rf bugsnag-native-ut-0.0.1.tgz CMakeUserPresets.json build
kepler build
popd
npm run local-npm:publish
