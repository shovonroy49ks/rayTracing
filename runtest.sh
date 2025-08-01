#!/bin/bash

# Check if a filename was provided
if [ -z "$1" ]; then
    echo "Usage: $0 <filename_without_extension>"
    exit 1
fi

name="$1"
rand_suffix=$(date +%s%N)
exe_folder="exe_files"
exe_name="${name}_${rand_suffix}"  # No .exe on Linux
exe_path="${exe_folder}/${exe_name}"

# Create folder if it doesn't exist
mkdir -p "$exe_folder"

# Build the compile command for Linux
compile_cmd="g++ $main.cpp -o ${exe_path} -lglut -lGLU -lGL"
run_cmd="./${exe_path}"

# Print and run the compile command
echo "$compile_cmd"
eval "$compile_cmd"

# Check for success
if [ $? -eq 0 ]; then
    echo "$run_cmd"
    eval "$run_cmd"
else
    echo "[Error] Compilation failed."
    exit 1
fi