// The MIT License (MIT)
//
// Copyright (c) 2015, Tamás Szelei
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#ifndef CONSTEXPR_MURMUR3_HASH_CONSTEXPR_MURMUR3_H
#define CONSTEXPR_MURMUR3_HASH_CONSTEXPR_MURMUR3_H

#include <stdint.h>

namespace ce_mm3 {

// Heavily based on sample code from
// http://en.cppreference.com/w/cpp/concept/LiteralType
class str_view {
public:
  constexpr str_view() : p(nullptr), sz(0) {}

  template <std::size_t N>
  constexpr str_view(const char(&a)[N])
      : p(a), sz(N - 1) {}

  constexpr char operator[](std::size_t n) const {
    return n < sz ? p[n] : throw std::out_of_range("");
  }

  constexpr std::size_t size() const { return sz; }

  constexpr const char *str() const { return p; }

private:
  const char *p;
  std::size_t sz;
};

// Calling __builtin_constant_p will force constant folding
#define ce_mm3_fold(x) __builtin_constant_p(x) ? (x) : (x)

constexpr uint32_t mm3_x86_32(const char *key, int len, uint32_t seed) {
  const int nblocks = len / 4;

  const uint8_t *data = ce_mm3_fold(reinterpret_cast<const uint8_t *>(key));
  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  const uint32_t *blocks = ce_mm3_fold((const uint32_t *)(data + nblocks * 4));

  for (int i = -nblocks; i; i++) {
    uint32_t k1 = blocks[i];

    k1 *= c1;
    k1 = (k1 << 15) | (k1 >> (32 - 15));
    k1 *= c2;

    h1 ^= k1;
    h1 = (h1 << 13) | (h1 >> (32 - 13));
    h1 = h1 * 5 + 0xe6546b64;
  }

  const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

  uint32_t k1 = 0;

  switch (len & 3) {
  case 3:
    k1 ^= tail[2] << 16;
  case 2:
    k1 ^= tail[1] << 8;
  case 1:
    k1 ^= tail[0];
    k1 *= c1;
    k1 = (k1 << 15) | (k1 >> (32 - 15));
    k1 *= c2;
    h1 ^= k1;
  };

  h1 ^= len;

  h1 ^= h1 >> 16;
  h1 *= 0x85ebca6b;
  h1 ^= h1 >> 13;
  h1 *= 0xc2b2ae35;
  h1 ^= h1 >> 16;

  return h1;
}

constexpr uint32_t mm3_x86_32(str_view s, uint32_t seed) {
  return mm3_x86_32(s.str(), s.size(), seed);
}

#undef ce_mm3_fold
}

#endif
