# Redis Implementation in C++

A C++ implementation of Redis-like server and client functionality.

## 🚧 Project Status
This project is currently under active development.

## 📋 Environment and Dependencies

- Ubuntu 24.04.1 or higher
- CMake 3.10 or higher
- g++ (GNU C++ Compiler) 13.3.0 or higher

## 🚀 Running the Project

You can manually compile and run the server and client files, or you can use the provided `run.sh` script for convenience. Follow the steps below to get started.

### Running the Script on Linux and macOS (Bash)

1. Open a terminal.
2. Navigate to the project directory.
3. Make the script executable (only needed once):
    ```bash 
    chmod +x run.sh
    ```
4. Run the script to build the project and start the server:
    ```bash
    ./run.sh
    ```
5. Execute the client with the desired command arguments:
    ```bash
    ./build/client.exe set key1 hello
    ```

### Available Commands
- `set <key> <value>`: Sets the value for the specified key.
- `get <key>`: Retrieves the value associated with the specified key.
- `del <key>`: Deletes the specified key.


## Credits
This project is based on the ["Build Your Own .org"](https://build-your-own.org/redis/) guide book by **James Smith**. It helped me understand low-level socket programming, client-server communication, and building a Redis-like key-value store.
