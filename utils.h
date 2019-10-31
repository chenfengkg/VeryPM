#pragma once
#include <x86intrin.h>
#include <atomic>
#include <cstdint>

#define IS_POWER_OF_TWO(x) (x && (x & (x - 1)) == 0)

inline uint64_t Murmur3_64(uint64_t h) {
  h ^= h >> 33;
  h *= 0xff51afd7ed558ccd;
  h ^= h >> 33;
  h *= 0xc4ceb9fe1a85ec53;
  h ^= h >> 33;
  return h;
}

template <typename T>
T CompareExchange64(T* destination, T new_value, T comparand) {
  static_assert(sizeof(T) == 8,
                "CompareExchange64 only works on 64 bit values");
  ::__atomic_compare_exchange_n(destination, &comparand, new_value, false,
                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  return comparand;
}

namespace pm_tool {
static const constexpr uint64_t kCacheLineSize = 64;

static void flush(void* addr) { _mm_clwb(addr); }
static void fence() { _mm_mfence(); }

template <typename T>
T CompareExchange64(T* destination, T new_value, T comparand) {
  static_assert(sizeof(T) == 8,
                "CompareExchange64 only works on 64 bit values");
  ::__atomic_compare_exchange_n(destination, &comparand, new_value, false,
                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
  return comparand;
}
}  // namespace pm_tool