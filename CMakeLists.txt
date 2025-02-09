cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Redis)

# Add the executable for client
add_executable(client.exe client.cpp)

# Add the executable for server
add_executable(server.exe server.cpp)