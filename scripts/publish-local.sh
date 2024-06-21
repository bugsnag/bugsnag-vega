#!/usr/bin/env bash
set -e

projectRoot="`dirname $0`/.."

if [[ "$VERSION_IDENTIFIER" == "" ]]; then
  VERSION_IDENTIFIER=$(node -p -e "require('$projectRoot/lerna.json').version")
  export VERSION_IDENTIFIER
fi

pushd "$projectRoot/packages/kepler-native"
npm run clean
rm -rf bugsnag-kepler-native-0.1.0.tgz CMakeUserPresets.json build
kepler build
popd
npm run local-npm:publish

pushd "$projectRoot/test/kepler-native-ut"
npm run clean
rm -rf bugsnag-native-ut-0.0.0.tgz CMakeUserPresets.json build
kepler build
npm run local-npm:publish
popd
