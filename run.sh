#!/bin/bash

# Function to clean up the server process
cleanup() {
    if [ -n "$SERVER_PID" ]; then
        kill $SERVER_PID
        wait $SERVER_PID 2>/dev/null
    fi
}

# Trap any exit signal to ensure cleanup is called
trap cleanup EXIT

# Create build directory if it doesn't exist
mkdir -p build

# Generate build files
cmake -B build

# Build the project
cmake --build build

# Check if the build was successful
if [ $? -eq 0 ]; then
    # Run the server executable in the background and store the process ID
    ./build/server.exe &
    SERVER_PID=$!

    # Wait for the server process to finish
    wait $SERVER_PID
else
    echo "Build failed."
fi