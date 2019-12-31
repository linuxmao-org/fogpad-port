#!/bin/bash
set -e

if test "$#" -ne 1; then
    echo "Usage: $0 <package-version>"
    exit 1
fi

if test ! -d scripts; then
    echo "Run this script from the project root."
    exit 1
fi

package_version="$1"

project_remote=$(git config --get remote.origin.url)
project_name=$(basename -s .git "$project_remote")

git-archive-all --prefix="$project_name"-"$package_version"/ "$project_name"-"$package_version".tar
zopfli "$project_name"-"$package_version".tar
rm -f "$project_name"-"$package_version".tar
echo "$project_name"-"$package_version".tar.gz
