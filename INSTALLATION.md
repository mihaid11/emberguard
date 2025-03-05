# Installation Guide for Emberguard

## Requirements:
- **CMake**: Version 3.10 or higher
- **Make**: (for Linux/macOS builds) or **Visual Studio** (for Windows builds)
- **SFML**: Version 2.6.2

1. Clone the repository:
   ```bash
   git clone https://github.com/mihaid11/Emberguard.git

2. Navigate to the project folder:
   ```bash
   cd Emberguard

3. Create a build directory:
   ```bash
   mkdir build
   cd build

4. Run CMake to configure the project:
   ```bash
   cmake ..

5. Build the project:
   ```bash
   cmake --build .
   ```

   Or
   ```bash
   make
   ```

6. Run the project
   On Linux:
   ```bash
   ./Emberguard
   ```

   On Windows:
   ```bash
   cd Debug
   Emberguard.exe
