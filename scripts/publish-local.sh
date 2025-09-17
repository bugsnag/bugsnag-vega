#!/usr/bin/env bash
set -e

projectRoot="`dirname $0`/.."

if [[ "$VERSION_IDENTIFIER" == "" ]]; then
  VERSION_IDENTIFIER=$(node -p -e "require('$projectRoot/lerna.json').version")
  export VERSION_IDENTIFIER
fi

pushd "$projectRoot/packages/kepler-native"
npm run clean
npm run build
popd
npm run local-npm:publish

pushd "$projectRoot/test/kepler-native-ut"
npm run clean
npm install
npm run local-npm:publish
popd
