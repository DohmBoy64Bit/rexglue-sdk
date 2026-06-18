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
| Vulkan-Headers | **v1.4.343** (`49f1a381`) | v1.4.321 (`2cd90f9d`) | ✅ ReXGlue ahead — no update needed |
| VMA (VulkanMemoryAllocator) | **v3.3.0** (`1d8f600f`) | **v3.3.0** (`1d8f600f`) | ✅ Identical commit |
| glslang | **SDK-candidate Jul 2020** (`f4f1d8a3`) | **16.0.0** (`a57276bf`) | 🔴 NEEDS UPDATE — 2020 pre-release vs 16.0.0 |
| SPIRV-Tools | **v2026.1** (`04d0b166`) | v2025.3 (`33e02568`) | ✅ ReXGlue ahead — no update needed |
| SPIRV-Headers | **vulkan-sdk-1.4.341.0** (`04f10f65`) | N/A (bundled in SPIRV-Tools) | ✅ Keep |
| volk | **1.4.304** (`0b17a763`) | Not used | ✅ ReXGlue-only |
| DirectXShaderCompiler | Minimal vendored headers (`thirdparty/dxc/include/`) | **v1.7.2308** (full submodule, `69e54e29`) | ⚠️ Import full submodule if D3D12 shader compilation path needs fixes |
| DirectX-Headers | Not present | **v1.618.2** (`33374754`) | ⚠️ Import if D3D12 backend sync requires new headers |
| SPIRV-Cross | Present | Not present | ✅ Keep |

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

1. **glslang**: Update from 2020 pre-release to 16.0.0 (Canary's version). Requires CMake fix:
   - `OGLCompiler` target removed (was in `OGLCompilersDLL/`)
   - `OSDependent` folded into `glslang` library (no separate target)
   - `MachineIndependent`/`GenericCodeGen` now empty stubs in new glslang
   - `glslang::SPIRV` alias now provided natively (remove compat fallback)
   - `SKIP_GLSLANG_INSTALL` → `GLSLANG_ENABLE_INSTALL OFF`
   - Must add `ENABLE_SPIRV ON` for SPIRV target to exist
2. **FFmpeg**: Do NOT swap submodules blindly. Diff XMA-related changes between forks first (ReXGlue: `wmarti/xenia-ffmpeg`; Canary: `has207/xmaframes`).
3. **DXC/DirectX-Headers**: Import full submodules only if D3D12 backend sync (Phase C) requires newer API headers than ReXGlue's minimal vendored headers.
4. **SDL**: Keep ReXGlue's SDL3 unless input sync (Phase G) reveals SDL2-specific fixes needed.
5. **Vulkan-Headers/SPIRV-Tools/VMA**: ReXGlue is already at or ahead of Canary — no changes needed.
