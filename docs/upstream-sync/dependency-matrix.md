# Dependency Matrix: ReXGlue vs Xenia Canary

Source: ReXGlue `e8ce24f` (v0.8.0) vs Xenia Canary `269ee61` (canary_experimental, 2026-06-18).

## Legend

| Symbol | Meaning |
|--------|---------|
| ✅ Keep | Use ReXGlue's version as-is |
| 🔄 Update | Import Canary's version after validation |
| ⚠️ Investigate | Needs manual comparison before decision |
| ❌ Skip | Canary's version not applicable to ReXGlue |

## Build System

| Component | ReXGlue | Xenia Canary | Decision |
|-----------|---------|-------------|----------|
| Build system | CMake 3.25+ | CMake + xb (Python) | ✅ Keep ReXGlue CMake |
| C++ standard | C++23 | C++20 | ✅ Keep ReXGlue C++23 |
| Compiler | Clang 18+ | Clang 19+ (Linux docs) | ⚠️ Update minimum if needed |
| Windows SDK | Latest | Latest | ✅ Keep |

## Graphics / GPU

| Component | ReXGlue | Xenia Canary | Decision |
|-----------|---------|-------------|----------|
| Vulkan Headers | KhronosGroup (submodule) | KhronosGroup (submodule) | 🔄 Update with shader/backend test pass |
| VMA | GPUOpen (submodule) | GPUOpen (submodule) | 🔄 Update only if Vulkan backend needs it |
| glslang | KhronosGroup (submodule) | KhronosGroup | ⚠️ Compare before update |
| SPIRV-Tools | KhronosGroup (submodule) | KhronosGroup | ⚠️ Compare before update |
| DirectXShaderCompiler | Not same layout | Present | 🔄 Import if D3D12 shader path needs it |
| DirectX-Headers | Not same layout | Present | 🔄 Import if D3D12 backend needs it |
| SPIRV-Cross | KhronosGroup (submodule) | Not present | ✅ Keep |

## Audio

| Component | ReXGlue | Xenia Canary | Decision |
|-----------|---------|-------------|----------|
| FFmpeg | `wmarti/xenia-ffmpeg` fork | `has207/xmaframes` fork | ⚠️ Compare XMA changes before replacing |
| XMA decoder | Custom (src/audio) | Custom (src/xenia/apu) | 🔄 Port behavior, not layout |

## Platform / Runtime

| Component | ReXGlue | Xenia Canary | Decision |
|-----------|---------|-------------|----------|
| SDL | SDL3 | SDL2 | ✅ Keep ReXGlue SDL3 unless input backend requires changes |
| Xbyak | Not present | Present (JIT) | ❌ Skip — ReXGlue is AOT/C++, not JIT |
| zstd | Not present | Present | ❌ Skip unless needed for trace format compat |
| zarchive | Not present | Present | ❌ Skip |
| fmtlib | Present | Present | ⚠️ Compare versions |
| spdlog | Present | Present | ⚠️ Compare versions |

## Third-Party Layout

| Aspect | ReXGlue | Xenia Canary |
|--------|---------|-------------|
| Root dir | `thirdparty/` | `third_party/` |
| Submodules | CMake FetchContent + git submodules | git submodules + manual |
| Policy | Prefer bundled/static linking | Mixed static/shared |

## Action Items

1. **Vulkan/SPIRV deps**: Update pinned versions to match Canary's to avoid symbol/shim conflicts during GPU sync.
2. **FFmpeg**: Do NOT swap submodules blindly. Diff XMA-related changes between forks first.
3. **DXC**: Import only if `src/graphics/d3d12` shader compilation path needs Canary's fixes.
4. **SDL**: Keep ReXGlue's SDL3 unless input sync (Phase G) reveals SDL2-specific fixes needed.
