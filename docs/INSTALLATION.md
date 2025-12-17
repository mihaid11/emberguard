# Installation Guide

## Requirements:
- **CMake**: >= 3.10
- **Make**: (Linux/macOS) or **Visual Studio** (Windows)
- **SFML**: 2.6.2

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

5. **Build the project**
   ```bash
   cmake --build .
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
