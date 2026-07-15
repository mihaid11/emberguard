# Installation Guide

## Requirements:
- **CMake**: >= 3.10
- **Make**: (Linux/macOS) or **Visual Studio** (Windows)
- **SFML**: 2.6.2
- **Catch2**: 3.15.2

## Build Instructions
1. **Clone the repository**
   ```bash
   git clone https://github.com/mihaid11/Emberguard.git
   ```

2. **Navigate to the project folder**
   ```bash
   cd Emberguard
   ```

3. **Create a build directory**
   ```bash
   mkdir build
   cd build
   ```

4. **Configure the project with CMake**
   ```bash
   cmake ..
   ```

   **Optional: If you want to compile the unit tests and benchmarks, enable the `BUILD_TESTS` flag**:
   ```bash
   cmake -DBUILD_TESTS=ON ..
   ```

5. **Build the project**
   ```bash
   cmake --build . -j4
   ```

   **Or alternatively**
   ```bash
   make
   ```

6. **Run the executable**

   On **Linux/macOS**:
   ```bash
   ./Emberguard
   ```

   On **Windows**:
   ```bash
   cd Debug
   Emberguard.exe
   ```

## Running tests and benchmarks
If you configured the project with `-DBUILD_TESTS=ON` in Step 4, an additional executable named `EmberguardTests` is generated.
To run all unit tests:
```bash
./EmberguardTests
```
