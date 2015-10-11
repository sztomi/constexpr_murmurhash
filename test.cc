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

#include "gtest/gtest.h"

#include "constexpr_murmur3.h"

using ce_mm3::mm3_x86_32;

// Strictly speaking, the values produced by this function should
// be testable with static_asserts, but I prefer gtest for the more
// friendly interface and error reporting.

// Test values are courtesy of StackOverflow user Ian Boyd
// See http://stackoverflow.com/a/31929528/140367

TEST(mm_x86_32_test, all_zeros_produce_zero) {
  EXPECT_EQ(0, mm3_x86_32("", 0));
}

TEST(mm_x86_32_test, zero_input_ignores_most_math) {
  EXPECT_EQ(0x514E28B7, mm3_x86_32("", 1));
}

TEST(mm_x86_32_test, seed_uses_unsigned_math) {
  EXPECT_EQ(0x81F16F39, mm3_x86_32("", 0xffffffff));
}

TEST(mm_x86_32_test, one_char) {
  EXPECT_EQ(0x7FA09EA6, mm3_x86_32("a", 0x9747b28c));
}

TEST(mm_x86_32_test, two_chars) {
  EXPECT_EQ(0x5D211726, mm3_x86_32("aa", 0x9747b28c));
}

TEST(mm_x86_32_test, three_chars) {
  EXPECT_EQ(0x283E0130, mm3_x86_32("aaa", 0x9747b28c));
}

TEST(mm_x86_32_test, four_chars) {
  EXPECT_EQ(0x5A97808A, mm3_x86_32("aaaa", 0x9747b28c));
}

TEST(mm_x86_32_test, endian_order_four_chars) {
  EXPECT_EQ(0xF0478627, mm3_x86_32("abcd", 0x9747b28c));
}

TEST(mm_x86_32_test, endian_order_three_chars) {
  EXPECT_EQ(0xC84A62DD, mm3_x86_32("abc", 0x9747b28c));
}

TEST(mm_x86_32_test, endian_order_two_chars) {
  EXPECT_EQ(0x74875592, mm3_x86_32("ab", 0x9747b28c));
}

TEST(mm_x86_32_test, endian_order_one_chunk) {
  const char input[] = {char(0x21), char(0x43), char(0x65), char(0x87)};
  EXPECT_EQ(0xF55B516B, mm3_x86_32(input, 4, 0));
}

TEST(mm_x86_32_test, special_seed_eliminates_initial_input) {
  const char input[] = {char(0x21), char(0x43), char(0x65), char(0x87)};
  EXPECT_EQ(0x2362F9DE, mm3_x86_32(input, 4, 0x5082EDEE));
}

TEST(mm_x86_32_test, hello_world) {
  EXPECT_EQ(0x24884CBA, mm3_x86_32("Hello, world!", 0x9747b28c));
}

TEST(mm_x86_32_test, utf8_characters) {
  EXPECT_EQ(0xD58063C1, mm3_x86_32("ππππππππ", 0x9747b28c));
}

TEST(mm_x86_32_test, long_str) {
  EXPECT_EQ(0xEE925B90,
            mm3_x86_32(
                "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 0));
}
