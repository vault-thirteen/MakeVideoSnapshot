#!/bin/bash

# This Shell Script compacts the 'stb' Folder, removing non-source-code Files.
# This Script must be executed from the 'scripts' Folder.

# Show each Command in std::out.
#set -x

# Forbid undefined Variables.
set -u

# Exit on Error.
set -e
set -o pipefail

# Settings.
LIB_FOLDER="stb"

# Compact the Folder.
rm -rf "../$LIB_FOLDER/data"
rm -rf "../$LIB_FOLDER/deprecated"
rm -rf "../$LIB_FOLDER/docs"
rm -rf "../$LIB_FOLDER/tests"
rm -rf "../$LIB_FOLDER/tools"
