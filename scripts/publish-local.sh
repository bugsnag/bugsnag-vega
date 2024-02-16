#!/usr/bin/env bash
if [[ "$VERSION_IDENTIFIER" == "" ]]; then
  export VERSION_IDENTIFIER="0.0.0"
fi

projectRoot="`dirname $0`/.."

pushd "$projectRoot/packages/kepler-native"
npm run clean
rm -rf bugsnag-kepler-native-0.0.0.tgz CMakeUserPresets.json build
kepler build
popd
npm run local-npm:publish
