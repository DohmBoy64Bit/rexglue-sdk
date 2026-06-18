# File Inventory: ReXGlue vs Xenia Canary

Base: ReXGlue `e8ce24f` (v0.8.0) · Xenia Canary `269ee61`
Generated: 2026-06-18

## Classification Buckets

| Bucket | Action | Visual |
|--------|--------|--------|
| **A — Direct Xenia-derived code** | Compare against Canary and port changes carefully | 🟦 |
| **B — ReXGlue-adapted Xenia logic** | Manually re-apply relevant upstream behavior | 🟩 |
| **C — ReXGlue-only AOT/recompiler code** | Do NOT replace with Xenia JIT/emulator code | 🟧 |
| **D — Third-party dependency glue** | Update only when needed | ⬜ |
| **E — Game/project-facing SDK API** | Preserve unless deliberate breaking-change plan | 🟪 |

---

## 1. `src/audio/` — Audio / XMA

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `audio_system.cpp` | 🟦 A | `src/xenia/apu/audio_system.cc` | Direct Xenia-derived audio system |
| `xma_context.cpp` | 🟩 B | `src/xenia/apu/xma_context.cc` | Adapted XMA context logic |
| `xma_decoder.cpp` | 🟩 B | `src/xenia/apu/xma_decoder.cc` | Adapted XMA decoder |
| `xma_register_file.cpp` | 🟩 B | `src/xenia/apu/xma_register_file.cc` | Adapted register file |

**Headers** (`include/rex/audio/xma/`):
| File | Bucket | Notes |
|------|--------|-------|
| `context.h` | 🟩 B | |
| `decoder.h` | 🟩 B | |
| `helpers.h` | 🟩 B | |
| `register_file.h` | 🟩 B | |
| `register_table.inc` | 🟩 B | |

---

## 2. `src/codegen/` — PPC Codegen (ReXGlue-only)

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `builders/helpers.h` | 🟧 C | — | ReXGlue AOT codegen helpers |
| `ppc/` (directory) | 🟧 C | `src/xenia/cpu/ppc/` | ReXGlue AOT vs Xenia JIT — only import instruction semantics |

**IT IS A MISTAKE to replace ReXGlue's codegen with Xenia's CPU/JIT.** ReXGlue is an AOT-generated-C++ recompiler; Xenia is a JIT emulator. Only port:
- PPC instruction semantic fixes
- Decoder/disassembler corrections
- Memory ordering/endian correctness
- FP/vector edge cases

---

## 3. `src/core/` — Core Utilities

| File | Bucket | Notes |
|------|--------|-------|
| `memory_posix.cpp` | 🟦 A | Xenia-derived memory mapping |

---

## 4. `src/filesystem/` — File System / VFS

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `vfs_dump.cpp` | 🟩 B | `src/xenia/kernel/util/vfs_dump.cc` | Adapted filesystem dump |

---

## 5. `src/graphics/` — GPU / Xenos (HIGHEST VALUE)

### `src/graphics/` — Common

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `command_processor.cpp` | 🟦 A | `src/xenia/gpu/command_processor.cc` | Direct PM4 command processor |
| `flags.cpp` | 🟩 B | `src/xenia/gpu/registers.cc` (flags enums) | Register flag definitions |
| `graphics_system.cpp` | 🟩 B | `src/xenia/gpu/graphics_system.cc` | Adapted graphics system |
| `packet_disassembler.cpp` | 🟦 A | `src/xenia/gpu/packet_disassembler.cc` | PM4 packet disassembler |
| `primitive_processor.cpp` | 🟦 A | `src/xenia/gpu/primitive_processor.cc` | Primitive processor |
| `register_file.cpp` | 🟦 A | `src/xenia/gpu/register_file.cc` | Xenos register file |
| `registers.cpp` | 🟦 A | `src/xenia/gpu/registers.cc` | Register table |
| `sampler_info.cpp` | 🟦 A | `src/xenia/gpu/sampler_info.cc` | Sampler info |
| `shared_memory.cpp` | 🟩 B | `src/xenia/gpu/shared_memory.cc` | Shared memory |
| `trace_dump.cpp` | 🟩 B | `src/xenia/gpu/trace_dump.cc` | Trace dump |
| `trace_player.cpp` | 🟩 B | `src/xenia/gpu/trace_player.cc` | Trace player |
| `trace_reader.cpp` | 🟩 B | `src/xenia/gpu/trace_reader.cc` | Trace reader |
| `trace_viewer.cpp` | 🟩 B | `src/xenia/gpu/trace_viewer.cc` | Trace viewer |
| `trace_writer.cpp` | 🟩 B | `src/xenia/gpu/trace_writer.cc` | Trace writer |
| `xenos.cpp` | 🟦 A | `src/xenia/gpu/xenos.cc` | Xenos GPU definitions |
| `video_mode_util.cpp` | 🟩 B | `src/xenia/gpu/video_mode_util.cc` | Video mode utility |

### `src/graphics/d3d12/` — D3D12 Backend

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `command_processor.cpp` | 🟦 A | `src/xenia/gpu/d3d12/command_processor.cc` | D3D12 command processor |
| `deferred_command_list.cpp` | 🟦 A | `src/xenia/gpu/d3d12/deferred_command_list.cc` | Deferred command list |
| `graphics_system.cpp` | 🟦 A | `src/xenia/gpu/d3d12/graphics_system.cc` | D3D12 graphics system |
| `pipeline_cache.cpp` | 🟦 A | `src/xenia/gpu/d3d12/pipeline_cache.cc` | Pipeline cache |
| `primitive_processor.cpp` | 🟦 A | `src/xenia/gpu/d3d12/primitive_processor.cc` | Primitive processor |
| `render_target_cache.cpp` | 🟦 A | `src/xenia/gpu/d3d12/render_target_cache.cc` | Render target cache |
| `shader.cpp` | 🟦 A | `src/xenia/gpu/d3d12/shader.cc` | D3D12 shader compilation |
| `shared_memory.cpp` | 🟩 B | `src/xenia/gpu/d3d12/shared_memory.cc` | Shared memory |
| `texture_cache.cpp` | 🟦 A | `src/xenia/gpu/d3d12/texture_cache.cc` | Texture cache |

### `src/graphics/vulkan/` — Vulkan Backend

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `command_processor.cpp` | 🟦 A | `src/xenia/gpu/vulkan/command_processor.cc` | Vulkan command processor |
| `deferred_command_buffer.cpp` | 🟩 B | `src/xenia/gpu/vulkan/deferred_command_buffer.cc` | Deferred command buffer |
| `graphics_system.cpp` | 🟦 A | `src/xenia/gpu/vulkan/graphics_system.cc` | Vulkan graphics system |
| `pipeline_cache.cpp` | 🟦 A | `src/xenia/gpu/vulkan/pipeline_cache.cc` | Pipeline cache |
| `primitive_processor.cpp` | 🟦 A | `src/xenia/gpu/vulkan/primitive_processor.cc` | Primitive processor |
| `render_target_cache.cpp` | 🟦 A | `src/xenia/gpu/vulkan/render_target_cache.cc` | Render target cache |
| `shader.cpp` | 🟦 A | `src/xenia/gpu/vulkan/shader.cc` | Vulkan shader compilation |
| `shared_memory.cpp` | 🟩 B | `src/xenia/gpu/vulkan/shared_memory.cc` | Shared memory |
| `texture_cache.cpp` | 🟦 A | `src/xenia/gpu/vulkan/texture_cache.cc` | Texture cache |

### `src/graphics/pipeline/` — Pipeline / Shaders

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `render_target/cache.cpp` | 🟦 A | `src/xenia/gpu/render_target_cache.cc` (or d3d12/) | Render target cache logic |
| `shader/dxbc_translator.cpp` | 🟩 B | `src/xenia/gpu/shader_translator.cc` (different arch) | DXBC translation — ReXGlue-adapted |
| `shader/interpreter.cpp` | 🟩 B | — | Shader interpreter |
| `shader/spirv_translator.cpp` | 🟩 B | `src/xenia/gpu/spirv_translator.cc` | SPIR-V translation — adapted |
| `shader/spirv_translator_fetch.cpp` | 🟩 B | — | SPIR-V fetch shader |
| `shader/spirv_translator_rb.cpp` | 🟩 B | — | SPIR-V render backend |
| `texture/cache.cpp` | 🟦 A | `src/xenia/gpu/texture_cache.cc` | Texture cache |

### `src/graphics/util/` — Graphics Utilities

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `draw.cpp` | 🟩 B | — | Draw utilities |
| `draw_extent_estimator.cpp` | 🟩 B | — | Draw extent estimation |

---

## 6. `src/input/` — Input / HID

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `input_system.cpp` | 🟩 B | `src/xenia/hid/input_system.cc` | Adapted input system |
| `mnk/mnk_input_driver.cpp` | 🟩 B | `src/xenia/hid/nop/nop_hid.cc` (approx) | MNK input driver |
| `nop/nop_input_driver.cpp` | 🟩 B | `src/xenia/hid/nop/nop_hid.cc` | No-op input driver |
| `sdl/sdl_input_driver.cpp` | 🟩 B | `src/xenia/hid/sdl/sdl_hid.cc` | SDL input driver (ReXGlue uses SDL3 vs Canary SDL2) |
| `xinput/xinput_input_driver.cpp` | 🟩 B | `src/xenia/hid/xinput/xinput_hid.cc` | XInput driver |

---

## 7. `src/kernel/` — Kernel / XAM / XBDM / xboxkrnl

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `crt/file.cpp` | 🟩 B | `src/xenia/kernel/util/file_utils.cc` (approx) | CRT file utils |
| `export_table_pre.inc` | 🟦 A | `src/xenia/kernel/export_table_pre.inc` | Export table |
| `xam/xam_app.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_app.cc` | XAM app |
| `xam/xam_debug.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_debug.cc` | XAM debug |
| `xam/xam_enum.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_enum.cc` | XAM enums |
| `xam/xam_info.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_info.cc` | XAM info |
| `xam/xam_input.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_input.cc` | XAM input |
| `xam/xam_msg.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_msg.cc` | XAM messaging |
| `xam/xam_notify.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_notify.cc` | XAM notifications |
| `xam/xam_task.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_task.cc` | XAM tasks |
| `xam/xam_user.cpp` | 🟩 B | `src/xenia/kernel/xam/xam_user.cc` | XAM user/profile |
| `xboxkrnl/export_table.inc` | 🟦 A | `src/xenia/kernel/xboxkrnl/export_table.inc` | xboxkrnl export table |
| `xboxkrnl/xboxkrnl_audio_xma.cpp` | 🟩 B | — (kernel + audio cross) | XMA audio kernel |
| `xboxkrnl/xboxkrnl_io_info.cpp` | 🟩 B | `src/xenia/kernel/xboxkrnl/xboxkrnl_io.cc` | I/O info |
| `xboxkrnl/xboxkrnl_io.cpp` | 🟩 B | `src/xenia/kernel/xboxkrnl/xboxkrnl_io.cc` | I/O |
| `xboxkrnl/xboxkrnl_memory.cpp` | 🟩 B | `src/xenia/kernel/xboxkrnl/xboxkrnl_memory.cc` | Memory |
| `xboxkrnl/xboxkrnl_threading.cpp` | 🟩 B | `src/xenia/kernel/xboxkrnl/xboxkrnl_threading.cc` | Threading |
| `xboxkrnl/xboxkrnl_video.cpp` | 🟩 B | `src/xenia/kernel/xboxkrnl/xboxkrnl_video.cc` | Video |

---

## 8. `src/system/` — System / Kernel State

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `kernel_state.cpp` | 🟩 B | `src/xenia/kernel/kernel_state.cc` | Kernel state |
| `xmemory.cpp` | 🟩 B | `src/xenia/kernel/xmemory.cc` | XBox memory |

---

## 9. `src/ui/` — UI / Overlay

| File | Bucket | Canary Counterpart | Notes |
|------|--------|--------------------|-------|
| `d3d12/d3d12_provider.cpp` | 🟩 B | — | D3D12 UI provider |
| `imgui_drawer.cpp` | 🟧 C | — | ImGui drawer (ReXGlue-specific) |
| `overlay/debug_overlay.cpp` | 🟧 C | `src/xenia/ui/debug_overlay.cc` (approx) | Debug overlay |

---

## 10. `include/rex/` — Public SDK API (🟪 E)

All headers under `include/rex/` are the **public-facing SDK API**. These must be preserved unless there is a deliberate breaking-change plan with versioning.

| Header | Notes |
|--------|-------|
| `rex/audio/xma/*` | XMA audio SDK |
| `rex/filesystem/devices/stfs_xbox.h` | STFS filesystem |
| `rex/graphics/*` | Graphics SDK |
| `rex/graphics/d3d12/*` | D3D12 SDK |
| `rex/graphics/vulkan/*` | Vulkan SDK |
| `rex/graphics/format/ucode.h` | Microcode format |
| `rex/graphics/pipeline/*` | Pipeline SDK |
| `rex/graphics/util/*` | Graphics utility SDK |
| `rex/hook.h` | Hook API |
| `rex/math.h` | Math API |
| `rex/perf/counter.h` | Performance counters |
| `rex/ppc/function.h` | PPC function API |
| `rex/system/*` | System/XEX API |
| `rex/thread/mutex.h` | Thread mutex API |
| `rex/ui/windowed_app.h` | Windowed app API |

---

## 11. `thirdparty/` — Third-Party Dependencies (⬜ D)

| Dependency | Decision | Notes |
|------------|----------|-------|
| `FFmpeg/` | ⚠️ Compare forks | ReXGlue: `wmarti/xenia-ffmpeg`; Canary: `has207/xmaframes` |
| `dxc/` | 🔄 Import if needed | Only if D3D12 shader path needs Canary fixes |
| `vulkan-headers/` | 🔄 Update | Update with shader/backend test pass |
| `vulkan-memory-allocator/` | 🔄 Update | Update if Vulkan backend sync requires it |
| `glslang/` | ⚠️ Compare | Compare version before update |
| `spirv-tools/` | ⚠️ Compare | Compare version before update |
| `fmt/`, `spdlog/` | ⚠️ Compare | Compare versions |
| All others | ✅ Keep | Not directly upstream-dependent |

---

## 12. `cmake/` — Build System (⬜ D)

| File | Decision | Notes |
|------|----------|-------|
| All `.cmake` files | ✅ Keep | ReXGlue's CMake build system; do not replace with Canary's `xb` |

## Summary

| Bucket | Count (est.) | Primary Sync Phase |
|--------|-------------|-------------------|
| 🟦 A — Direct Xenia-derived | ~30+ files | Phase B/C/D (GPU), E (kernel tables) |
| 🟩 B — ReXGlue-adapted | ~35+ files | Phase B/C/D/E/F/G (manual port) |
| 🟧 C — ReXGlue-only | ~20+ files | Phase H (codegen) — least touch |
| ⬜ D — Third-party glue | ~5-10 deps | Phase A |
| 🟪 E — Public SDK API | ~40+ headers | Preserve across all phases |
