# MinGW cross-compilation toolchain file
# Enables building a Windows .exe from macOS using:
#   brew install mingw-w64
#   mkdir build-windows && cd build-windows
#   cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw.cmake
#   make

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Detect Homebrew prefix (Apple Silicon uses /opt/homebrew, Intel uses /usr/local)
if(EXISTS /opt/homebrew/bin/x86_64-w64-mingw32-gcc)
    set(_MINGW_PREFIX /opt/homebrew)
elseif(EXISTS /usr/local/bin/x86_64-w64-mingw32-gcc)
    set(_MINGW_PREFIX /usr/local)
else()
    message(FATAL_ERROR
        "MinGW cross-compiler not found. Install it with: brew install mingw-w64")
endif()

set(CMAKE_C_COMPILER   ${_MINGW_PREFIX}/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${_MINGW_PREFIX}/bin/x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER  ${_MINGW_PREFIX}/bin/x86_64-w64-mingw32-windres)

# Point CMake at the MinGW sysroot so it can find Windows libraries/headers
set(CMAKE_FIND_ROOT_PATH
    ${_MINGW_PREFIX}/opt/mingw-w64/toolchain-x86_64/x86_64-w64-mingw32
)

# Search host system only for build tools (programs), never for target libraries/headers.
# Libraries and headers are searched exclusively in the MinGW sysroot (CMAKE_FIND_ROOT_PATH).
# For SDL3, pass -DSDL3_DIR=<path-to-windows-mingw-sdl3> when running cmake.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
