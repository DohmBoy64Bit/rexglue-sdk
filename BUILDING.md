# ReXGlue Canary — Build Guide

This guide covers building the **rexglue-canary** SDK from source and using it to
build a ReXGlue port project. The Outfit XBLA port is used as the worked example.

## Prerequisites

| Tool | Required For | How to get |
|------|-------------|------------|
| Visual Studio 2022 | C++ compiler, linker, Windows SDK | [visualstudio.com](https://visualstudio.microsoft.com/) — install **Desktop development with C++** workload |
| CMake 3.25+ | Build system | Bundled with VS, or [cmake.org](https://cmake.org/) |
| Ninja | Build executor | Bundled with VS, or `winget install Ninja-build.Ninja` |
| Git | Source control + submodules | [git-scm.com](https://git-scm.com/) |

Verify everything is reachable:

```powershell
# From Developer PowerShell for VS 2022:
clang-cl --version
cmake --version
ninja --version
```

## 1. Clone rexglue-canary

```powershell
git clone --recurse-submodules https://github.com/DohmBoy64Bit/rexglue-sdk.git rexglue-canary
cd rexglue-canary
git checkout sync/xenia-canary-269ee61
```

## 2. Build the SDK

All commands below run from **Developer PowerShell for VS 2022** (right-click, *Terminal* → *Developer PowerShell*).

### D3D12-only (simpler, fewer dependencies)

```powershell
cd E:\rexglue-canary

# Configure
cmake -B build -G Ninja `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl

# Build
cmake --build build --config Debug -j8
```

### D3D12 + Vulkan (full graphics support)

```powershell
cd E:\rexglue-canary

# Configure
cmake -B build -G Ninja `
    -DREXGLUE_USE_VULKAN=ON `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl

# Build (--target rexgraphics rexruntime rexglue to skip spirv-tools)
cmake --build build --config Debug -j8 --target rexgraphics rexruntime rexglue
```

> **Note:** The Vulkan build may show `spirv-tools` `-Werror` warnings treated as errors.
> This is a pre-existing clang-cl flag incompatibility in the third-party spirv-tools
> library. The core SDK targets (`rexgraphics`, `rexruntime`, `rexglue`) compile fine.
> Use `--target` to skip spirv-tools if needed.

### Known issues during build

| Error | Cause | Fix |
|-------|-------|-----|
| `expected identifier or '('` in `cabextract/mspack/*.c` | Symlink → text file on Windows | See [mspack workaround](#mspack-symlink-workaround) |
| `char8_t` → `string_view` conversion in `xboxkrnl_ob.cpp` | C++20 `u8` literal type mismatch | Change `u8"\\??\\"` to `"\\??\\"` (ASCII-only string) |
| `"C:/…/ld.exe" cannot find /nologo` | MingW `ld.exe` shadowing MSVC `link.exe` | Remove MingW from PATH or pass `-DCMAKE_LINKER="C:/…/link.exe"` |
| `clang is not a full path` | Outside dev prompt | Run from **Developer PowerShell for VS 2022** |

### mspack symlink workaround

```powershell
cd E:\rexglue-canary
$real = "thirdparty/libmspack/libmspack/mspack"
$link = "thirdparty/libmspack/cabextract/mspack"
@("lzxd.c","cabd.c","mszipd.c","qtmd.c","system.c",
  "cab.h","lzx.h","macros.h","mspack.h","mszip.h",
  "qtm.h","readbits.h","readhuff.h","system.h") | ForEach-Object {
    Copy-Item "$real/$_" "$link/$_" -Force
}
```

Then rebuild.

### Verify the build

```powershell
Get-ChildItem out/win-amd64 -Include *.exe,*.dll | Select-Object Name,Length
# rexglue.exe       ~5 MB
# rexruntimed.dll  ~24 MB
# TracyClientd.dll  ~1 MB
```

## 3. Install the SDK

```powershell
cmake --install build --config Debug --prefix out/install
```

This places everything under `out/install/`:

```
out/install/
  include/rex/          # Public headers
  lib/                  # Static libs
  lib/cmake/rexglue/    # CMake find_package config
  bin/                  # rexglue.exe, rexruntimed.dll
```

CMake also registers the SDK in your user package registry so `find_package(rexglue)`
finds it automatically.

## 4. Build a port project (example: The Outfit)

The Outfit is an XBLA delisted game ported to ReXGlue.

### Get the port

```powershell
git clone https://github.com/DohmBoy64Bit/TheOutFit.git D:\360RexGlue\TheOutFit
```

Place your game files in `D:\360RexGlue\TheOutFit\assets\game_files\` (not included).

### Configure

```powershell
cd D:\360RexGlue\TheOutFit\TheOutFit_Port

cmake -B out/build/win-amd64-debug -G Ninja `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl `
    -DCMAKE_PREFIX_PATH=E:/rexglue-canary/out/install
```

You should see:

```
Found ReXGlue SDK 0.8.1.x at E:/rexglue-canary/out/install/lib/cmake/rexglue
```

### Build

```powershell
cmake --build out/build/win-amd64-debug --config Debug -j8
```

### Output

```
out/build/win-amd64-debug/
  theoutfit.exe        ~75 MB   # Host executable
  rexruntimed.dll      ~24 MB   # SDK runtime (auto-staged)
  TracyClientd.dll     ~1 MB    # Profiler (auto-staged)
```

### Run

```powershell
.\out\build\win-amd64-debug\theoutfit.exe
```

## 5. Using a different port

The same pattern works for any rexglue port. The port's `generated/rexglue.cmake`
handles SDK discovery automatically. You only need to point it at your SDK install:

```powershell
cd path\to\your-port
cmake -B out/build/win-amd64-debug -G Ninja `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl `
    -DCMAKE_PREFIX_PATH=E:/rexglue-canary/out/install

cmake --build out/build/win-amd64-debug --config Debug -j8
```

## 6. Rebuilding after SDK changes

### Rebuild SDK only

```powershell
cd E:\rexglue-canary
cmake --build build --config Debug -j8
cmake --install build --config Debug --prefix out/install
```

### Rebuild port only (no reconfigure needed)

```powershell
cd D:\360RexGlue\TheOutFit\TheOutFit_Port
cmake --build out/build/win-amd64-debug --config Debug -j8
```

### Full clean rebuild

```powershell
# SDK
Remove-Item -Recurse -Force E:\rexglue-canary\build, E:\rexglue-canary\out -ErrorAction SilentlyContinue
cd E:\rexglue-canary
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl
cmake --build build --config Debug -j8
cmake --install build --config Debug --prefix out/install

# Port
Remove-Item -Recurse -Force D:\360RexGlue\TheOutFit\TheOutFit_Port\out\build -ErrorAction SilentlyContinue
cd D:\360RexGlue\TheOutFit\TheOutFit_Port
cmake -B out/build/win-amd64-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_PREFIX_PATH=E:/rexglue-canary/out/install
cmake --build out/build/win-amd64-debug --config Debug -j8
```

## 7. Troubleshooting

| Symptom | Likely cause | Fix |
|---------|-------------|-----|
| `find_package(rexglue)` fails | SDK not installed or wrong path | Re-run `cmake --install`, check `CMAKE_PREFIX_PATH` |
| `rex/version.h` not found | Missing `build/include` path | Use installed SDK (`CMAKE_PREFIX_PATH`) not source tree (`REXSDK_DIR`) |
| `imgui.h` not found | Same as above | Use installed SDK |
| Port build pulls in wrong SDK version | `generated/rexglue.cmake` has old version pin | Update `REXSDK_VERSION` or remove the version pin |
| Linker can't find `rexruntimed.lib` | SDK stale after changes | Re-run `cmake --install build --prefix out/install` |
| Game crashes on launch | Missing game assets | Ensure `assets\game_files\` exists with game data |
