# Building ReXGlue from Source

## Prerequisites

- **Visual Studio 2022** (Community or higher) with C++ Desktop workload
- **CMake** 3.25+ (included with VS, or standalone)
- **Ninja** (included with VS, or standalone)
- **Git for Windows** (for submodules)

## 1. Clone and set up

```powershell
git clone --recurse-submodules https://github.com/rexglue/rexglue-sdk.git
cd rexglue-sdk
```

## 2. Build the SDK

Open **Developer PowerShell for VS 2022** (or run `VsDevCmd.bat` first), then:

```powershell
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl

cmake --build build --config Debug -j8
```

### Windows symlink workaround

Some third-party libraries (mspack) use symlinks that break on Windows. If you see `expected identifier or '('` errors in `cabextract/mspack/*.c` files, copy the real sources:

```powershell
$real = "thirdparty/libmspack/libmspack/mspack"
$link = "thirdparty/libmspack/cabextract/mspack"
@("lzxd.c","cabd.c","mszipd.c","qtmd.c","system.c",
  "cab.h","lzx.h","macros.h","mspack.h","mszip.h","qtm.h","readbits.h","readhuff.h","system.h") | ForEach-Object {
    Copy-Item "$real/$_" "$link/$_" -Force
}
```

Then rebuild.

## 3. Install the SDK

```powershell
cmake --install build --config Debug --prefix out/install
```

This places headers, libs, and CMake config under `out/install/`.

## 4. Build a port project against the SDK

Use `CMAKE_PREFIX_PATH` to point the port project at your installed SDK:

```powershell
cd D:\path\to\your-port

cmake -B out/build/win-amd64-debug -G Ninja `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl `
    -DCMAKE_PREFIX_PATH=E:/path/to/rexglue-sdk/out/install

cmake --build out/build/win-amd64-debug --config Debug -j8
```

The port's `generated/rexglue.cmake` will auto-detect the SDK via `find_package(rexglue)`.

### Alternative: source-tree SDK

If you prefer building the SDK alongside the port (e.g., if you're modifying SDK code), pass `-DREXSDK_DIR=E:/path/to/rexglue-sdk` instead of `-DCMAKE_PREFIX_PATH`. This adds the SDK as a CMake subdirectory. Note: you may need to add extra include paths (imgui, build/include) to the port's CMakeLists.txt in this mode.

## 5. Output

The port build produces:

```
out/build/win-amd64-debug/
  yourport.exe        # host executable
  rexruntimed.dll     # SDK runtime (auto-staged)
  TracyClientd.dll    # profiler (auto-staged)
```

## Quick rebuild (no reconfigure)

```powershell
cmake --build out/build/win-amd64-debug --config Debug -j8
```

## Common issues

| Symptom | Fix |
|---------|-----|
| `'imgui.h' file not found` | Missing imgui include. Use installed SDK via `CMAKE_PREFIX_PATH`. |
| `'rex/version.h' file not found` | Missing build/include. Same - use installed SDK. |
| mspack `expected identifier or '('` | Symlink redirect issue. Run the workaround above. |
| `char8_t` conversion errors | Pre-existing. Known in `xboxkrnl_ob.cpp`. Drop the `u8` prefix on ASCII-only strings. |
| `clang is not a full path` | Set `CMAKE_C_COMPILER=clang-cl` and `CMAKE_CXX_COMPILER=clang-cl`. |
