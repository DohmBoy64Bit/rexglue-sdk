# Upstream Sync: Xenia Canary `269ee61`

## Source Metadata

| Field | Value |
|-------|-------|
| **Date** | 2026-06-18 |
| **ReXGlue base commit** | `e8ce24fa73cd7c1ede80262c06f34893b7963dbe` (v0.8.0) |
| **Xenia Canary commit** | `269ee6184595d35979136993174c0109efae5c5f` |
| **Xenia Canary branch** | `canary_experimental` |
| **Xenia Canary HEAD message** | `[XAM] Fixed returning error in XamLoaderGetLaunchData` |

## Sync Rules

These rules govern every change imported from Xenia Canary into ReXGlue:

### Legal / Source Discipline

1. **No XDK-derived information.** Xenia's contribution rules explicitly reject XDK-derived information. ReXGlue must maintain the same standard. Every behavioral change must trace to documented guest-observable behavior, not leaked SDK internals.
2. **Every imported behavior must cite source.** Each ported change must reference the Xenia Canary source file and commit hash it was derived from.
3. **Every approximation must be marked.** Any behavior that is guessed, inferred, or approximated must be annotated with `TODO` or `HYPOTHESIS` in the code and documented here.

### Patching Discipline

4. **Never wholesale-replace files.** Import changes file-by-file or function-by-function, preserving ReXGlue's naming, layout, and code conventions.
5. **No game-specific hacks.** Do not import game-specific workarounds from Xenia Canary unless they demonstrate correct general behavior per the Xenia specification.
6. **Document deviations.** Every deviation from Xenia Canary's behavior (intentional or otherwise) must be recorded in this document.

## Deviation Register

| # | Subsystem | Deviation | Rationale | Date |
|---|-----------|-----------|-----------|------|
| 1 | glslang CMake | Removed OGLCompiler/OSDependent link deps | Folded into glslang lib in 16.0.0 | 2026-06-18 |
| 2 | glslang CMake | Removed MachineIndependent/GenericCodeGen link deps | Empty stubs in 16.0.0 | 2026-06-18 |
| 3 | glslang CMake | Removed glslang::SPIRV alias compat block | Provided natively in 16.0.0 | 2026-06-18 |

## Import Log

| Date | Subsystem | File(s) | Canary Commit | Reason | Status |
|------|-----------|---------|---------------|--------|--------|
| 2026-06-18 | deps/glslang | `thirdparty/glslang`, `thirdparty/CMakeLists.txt`, `src/graphics/CMakeLists.txt` | a57276bf (16.0.0) | Update from 2020 pre-release to match Canary's glslang version. Required CMake option renames and link dependency removal. | ✅ Complete |
| 2026-06-18 | deps/verification | — | — | CMake configure verified with D3D12 backend on Clang 22.1.6. Build blocked by pre-existing SSSE3 simde issue (unrelated). | ⚠️ Configure OK |
| 2026-06-18 | Phase B1: register layer | `xenos.h`, `register_table.inc`, `register_file.h/cpp` | 269ee61 | Renamed 5 UNKNOWN registers, added `IsValidRegister()` with compile-time bitset, added `kUnused1/2/3` to `PrimitiveType` | ✅ Complete |
| 2026-06-18 | Phase B2: PM4 decode | `packet_disassembler.h/cpp`, `trace_viewer.cpp` | 269ee61 | Expanded `PacketAction` from 3 to 25 types, 6 new opcode handlers, `constexpr`/`reserve()` optimizations | ✅ Complete |
| 2026-06-18 | Phase B3: format helpers | `xenos.cpp` | 269ee61 | Added `GetEndianEnglishDescription()` and `GetPrimitiveTypeEnglishDescription()` with lookup tables | ✅ Complete |
| 2026-06-18 | Phase B4: blend pre-multiply | `dxbc_translator.h`, `dxbc_translator_om.cpp`, `d3d12/pipeline_cache.cpp` | 269ee61 | MIN/MAX blend op factor pre-multiply in `CompletePixelShader_WriteToRTVs()` — compensates for D3D12 ignoring blend factors on MIN/MAX ops | ✅ Complete |
| 2026-06-18 | Phase B4: ZPD ROV counter | `dxbc_translator.h`, `dxbc_translator.cpp`, `dxbc_translator_om.cpp` | 269ee61 | ZPD counter shader layer: new system constant, UAV register, `ROV_AddPassedMSAASamplesToZPD()` impl with atomic-add. Backend wiring pending. | ✅ Shader layer done |
| 2026-06-18 | Phase B3: sampler_info | `sampler_info.cpp` | 269ee61 | Zero semantic drift — no changes needed | ✅ Skip |
| 2026-06-19 | build: SSSE3 | `CMakeLists.txt` | — | Added global `-mssse3` flag — multiple files use `_mm_shuffle_epi8` and all x86-64 CPUs support SSSE3 | ✅ Complete |
| 2026-06-19 | DXBC opcodes | `dxbc.h` | 269ee61 | Added `kCountBits` (0xB0) and `kAtomicIAdd` (0xC5) to Opcode enum plus `OpCountBits()`/`OpAtomicIAdd()` assembler methods | ✅ Complete |
| 2026-06-19 | D3D12 ZPD backend | `command_processor.h/cpp` | 269ee61 | ZPD ROV counter full lifecycle: `InitializeZpdRovCounterResources()`/`Shutdown...()`, `AcquireZpdCounterIndex()`, `Begin.../EndZpdRovCounterSegment()`, root signature 4th descriptor range, event handler dispatch | ✅ Complete |
| 2026-06-19 | D3D12 fix | `command_processor.cpp` | — | Fixed 4 `D3DResourceBarrier` calls from 3-arg to proper `D3D12_RESOURCE_BARRIER` struct + `(1, &barrier)` | ✅ Complete |
| 2026-06-19 | SPIRV blend premult | `spirv_translator.h`, `spirv_translator_rb.cpp` | 269ee61 | Ported blend pre-multiply fields to SPIRV `PixelShaderModification` (reordered bits, shrunk `depth_stencil_mode` 3→2, bumped `kVersion` 12→13); 12-case RGB + 7-case alpha switch in `spirv_translator_rb.cpp` | ✅ Complete |
| 2026-06-19 | SPIRV ZPD counter | `spirv_translator.h`, `spirv_translator.cpp`, `spirv_translator_rb.cpp` | 269ee61 | Added `zpd_rov_counter_index` system constant, ZPD SSBO at binding 2, `OpBitCount`+`OpAtomicIAdd` counter update in FSI path | ✅ Complete |
| 2026-06-19 | SPIRV: glslang 16.0.0 migration | `spirv_translator*.cpp`, `spirv_builder.h/cpp` | — | Migrated 7 files from old flat-enum to scoped-enum API (`spv::OpBitcast`→`spv::Op::OpBitcast`); fixed `IfBuilder`/`SwitchBuilder` control types, `createBranch(Block*)`→`createBranch(bool,Block*)`, `makeFunctionEntry`+`LinkageType`, `BuiltIn` casts, regex-corrupted `ExecutionModel` recovery | ✅ Complete |
| 2026-06-19 | glslang install fix | `cmake/rexglue_install.cmake` | — | Removed stale `OGLCompiler` from install target list (removed in glslang 16.0.0) | ✅ Complete |
| 2026-06-19 | Vulkan pipeline cache | `vulkan/pipeline_cache.cpp` | 269ee61 | Added blend pre-multiply factor population to `GetCurrentPixelShaderModification()` matching D3D12 path; SPIRV enum migration | ✅ Complete |
| 2026-06-19 | Vulkan build fix | `vulkan/command_processor.cpp` | — | Added 9 missing EXT mesh shader fields to `TBuiltInResource` initializer (added in glslang 16.0.0) | ✅ Complete |
