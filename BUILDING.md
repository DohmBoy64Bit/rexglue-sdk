# Building Against rexglue-canary

This guide covers building a ReXGlue port project using the `rexglue-canary` SDK (this repo).

## Prerequisites

- **Visual Studio 2022** with C++ Desktop workload
- **CMake** 3.25+
- **Ninja** (bundled with VS)
- **Git for Windows**

## 1. Build rexglue-canary

Open **Developer PowerShell for VS 2022** (or run `VsDevCmd.bat -arch=x64` first), then:

```powershell
cd E:\rexglue-canary

# Configure (D3D12-only)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl

# Build
cmake --build build --config Debug -j8
```

### Symlink workaround (mspack)

Some third-party libraries ship redirect files instead of symlinks. If `cabextract/mspack/*.c` files fail to compile:

```powershell
cd E:\rexglue-canary
$r = "thirdparty/libmspack/libmspack/mspack"
$l = "thirdparty/libmspack/cabextract/mspack"
@("lzxd.c","cabd.c","mszipd.c","qtmd.c","system.c",
  "cab.h","lzx.h","macros.h","mspack.h","mszip.h",
  "qtm.h","readbits.h","readhuff.h","system.h") | ForEach-Object {
    Copy-Item "$r/$_" "$l/$_" -Force
}
```

### Pre-existing kernel fix

One kernel file has a C++20 `char8_t` issue. If `xboxkrnl_ob.cpp` fails:

```cpp
// change:   u8"\\??\\"
//    to:    "\\??\\"
```

The string is pure ASCII so dropping the `u8` prefix is safe.

## 2. Install the SDK

```powershell
cd E:\rexglue-canary
cmake --install build --config Debug --prefix out/install
```

Output goes to `E:\rexglue-canary\out\install\`. CMake also registers the SDK in your user package registry.

## 3. Configure a port project

```powershell
cd D:\360RexGlue\TheOutFit\TheOutFit_Port

cmake -B out/build/win-amd64-debug -G Ninja `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl `
    -DCMAKE_PREFIX_PATH=E:/rexglue-canary/out/install
```

The port's `generated/rexglue.cmake` finds the SDK via `find_package(rexglue)`. You should see:

```
Found ReXGlue SDK 0.8.1.x at E:/rexglue-canary/out/install/lib/cmake/rexglue
```

### Option B: source-tree SDK (for SDK hacking)

If you're iterating on SDK code and the port simultaneously, build from the source tree instead of the install:

```powershell
cmake -B out/build/win-amd64-debug -G Ninja `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl `
    -DREXSDK_DIR=E:/rexglue-canary
```

This adds the SDK as a CMake subdirectory. Downside: you may need to patch the port's CMakeLists.txt with extra include paths.

## 4. Build the port

```powershell
cd D:\360RexGlue\TheOutFit\TheOutFit_Port
cmake --build out/build/win-amd64-debug --config Debug -j8
```

## 5. Output

```
out/build/win-amd64-debug/
  theoutfit.exe        ~75 MB   # host executable
  rexruntimed.dll      ~24 MB   # SDK runtime (auto-staged)
  TracyClientd.dll     ~0.6 MB  # profiler (auto-staged)
```

Place your game assets in `assets\game_files\` (relative to the exe, or per your port's README).

## Quick rebuild

```powershell
# Port only (no reconfigure needed)
cmake --build out/build/win-amd64-debug --config Debug -j8

# SDK only
cmake --build E:/rexglue-canary/build --config Debug -j8 --target rexgraphics
```

## Vulkan variant

To enable the Vulkan backend:

```powershell
cd E:\rexglue-canary
cmake -B build -G Ninja -DREXGLUE_USE_VULKAN=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl
cmake --build build --config Debug -j8
cmake --install build --config Debug --prefix out/install
```

Then reconfigure the port with the same `CMAKE_PREFIX_PATH`.
