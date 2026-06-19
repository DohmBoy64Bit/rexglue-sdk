/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <array>
#include <cstring>

#include <rex/graphics/register_file.h>
#include <rex/math.h>

namespace rex::graphics {

RegisterFile::RegisterFile() {
  std::memset(values, 0, sizeof(values));
}

constexpr unsigned int GetHighestRegisterNumber() {
  uint32_t highest = 0;
#define XE_GPU_REGISTER(index, type, name) \
  if (index > highest) { highest = index; }
#include <rex/graphics/register_table.inc>
#undef XE_GPU_REGISTER
  return highest;
}

constexpr unsigned int GetLowestRegisterNumber() {
  uint32_t lowest = UINT_MAX;
#define XE_GPU_REGISTER(index, type, name) \
  if (index < lowest) { lowest = index; }
#include <rex/graphics/register_table.inc>
#undef XE_GPU_REGISTER
  return lowest;
}

static constexpr uint32_t lowest_register = GetLowestRegisterNumber();
static constexpr uint32_t highest_register = GetHighestRegisterNumber();
static constexpr uint32_t total_num_registers = highest_register - lowest_register;
static constexpr uint32_t num_required_words_for_registers =
    ((total_num_registers + 63) & ~63) / 64;

using ValidRegisterBitset =
    std::array<uint64_t, num_required_words_for_registers>;

static constexpr ValidRegisterBitset BuildValidRegisterBitset() {
  ValidRegisterBitset result{};
#define XE_GPU_REGISTER(index, type, name)    \
  result[(index - lowest_register) / 64] |=   \
      1ULL << ((index - lowest_register) % 64);
#include <rex/graphics/register_table.inc>
#undef XE_GPU_REGISTER
  return result;
}

static constexpr ValidRegisterBitset valid_register_bitset =
    BuildValidRegisterBitset();

const RegisterInfo* RegisterFile::GetRegisterInfo(uint32_t index) {
  switch (index) {
#define XE_GPU_REGISTER(index, type, name) \
  case index: {                            \
    static const RegisterInfo reg_info = { \
        RegisterInfo::Type::type,          \
        #name,                             \
    };                                     \
    return &reg_info;                      \
  }
#include <rex/graphics/register_table.inc>
#undef XE_GPU_REGISTER
    default:
      return nullptr;
  }
}

bool RegisterFile::IsValidRegister(uint32_t index) {
  if (index < lowest_register) [[unlikely]] {
    return false;
  }
  if (index > highest_register) [[unlikely]] {
    return false;
  }
  uint32_t register_linear_index = index - lowest_register;
  return (valid_register_bitset[register_linear_index / 64] &
          (1ULL << (register_linear_index % 64))) != 0;
}

}  // namespace rex::graphics
