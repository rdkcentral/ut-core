#!/bin/bash

# Check if a directory path is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <directory>"
  exit 1
fi

# Collect all .c, .cpp, and .s files from the directories passed as arguments
find "$@" -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.s" \)
