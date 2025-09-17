#!/usr/bin/env bash

set -euxo pipefail

error_missing_field () {
  echo "Missing required env var: $1"
  exit 1
}

# Ensure all required variables are set before doing any work
if [[ -z ${GITHUB_USER:-} ]]; then error_missing_field "GITHUB_USER"; fi
if [[ -z ${GITHUB_ACCESS_TOKEN:-} ]]; then error_missing_field "GITHUB_ACCESS_TOKEN"; fi
if [[ -z ${RELEASE_BRANCH:-} ]]; then error_missing_field "RELEASE_BRANCH"; fi
if [[ -z ${VERSION:-} ]]; then error_missing_field "VERSION"; fi
if [[ -z ${DIST_TAG:-} ]]; then error_missing_field "DIST_TAG"; fi

# Ensure current branch mathches the expected release branch
current_branch=$(git rev-parse --abbrev-ref HEAD)
if [[ "$current_branch" != "$RELEASE_BRANCH" ]]; then
  echo "Current branch '$current_branch' does not match expected release branch '$RELEASE_BRANCH'"
  exit 1
fi

# Ensure git working tree is clean
if [[ -n $(git status --porcelain) ]]; then
  echo "Git working tree is not clean, please commit or stash any changes before running this script"
  exit 1
fi

# "ci" rather than "install" ensures the process doesn't make the work tree dirty by modifying lockfiles
npm ci

# increment package version numbers
if [ -z "${RETRY_PUBLISH:-}" ]; then
  # ensure we don't use legacy peer deps when bumping versions (https://github.com/nrwl/nx/issues/22066)
  npm_config_legacy_peer_deps=false npx lerna version "$VERSION"  --no-push --no-private
fi

npm run build

# push version bump commit and tags
git push --follow-tags

# publish
# publish
if [ -v RETRY_PUBLISH ]; then
  npx lerna publish from-package --dist-tag "$DIST_TAG"
else
  npx lerna publish from-git --dist-tag "$DIST_TAG"
fi