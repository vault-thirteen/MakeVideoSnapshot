#!/bin/bash

# This Shell Script downloads the Library which writes (encodes) the PNG Files.
# This Script must be executed from the 'scripts' Folder.

# Show each Command in std::out.
#set -x

# Forbid undefined Variables.
set -u

# Exit on Error.
set -e
set -o pipefail

# Settings.
REPOSITORY="https://github.com/nothings/stb"
LIB_FOLDER="stb"

# Download the Code.
git clone $REPOSITORY
mkdir "../$LIB_FOLDER"
mv -v $LIB_FOLDER/* ../$LIB_FOLDER
rm -rf $LIB_FOLDER
