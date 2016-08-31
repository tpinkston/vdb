// ============================================================================
// VDB (VDIS Debugger)
// Tony Pinkston (git@github.com:tpinkston/vdb.git)
//
// VDB is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// VDB is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details (http://www.gnu.org/licenses).
// ============================================================================

#include "vdb_binary.h"
#include "vdb_bits.h"
#include "vdb_byte_stream.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_string.h"
#include "vdb_test.h"

#ifdef __CYGWIN__
int test_common_cygwin(void);
#endif
int test_common_binary(void);
int test_common_hexadecimal(void);
int test_common_logger(void);
int test_common_string(void);
int test_common_byte_stream(void);
int test_common_bits(void);

// ----------------------------------------------------------------------------
void vdb::test::test_common(void)
{
#ifdef __CYGWIN__
    failure_counter += test_common_cygwin();
#endif

    failure_counter += test_common_logger();
    failure_counter += test_common_binary();
    failure_counter += test_common_hexadecimal();
    failure_counter += test_common_string();
    failure_counter += test_common_byte_stream();
    failure_counter += test_common_bits();

    std::cout << "sizeof(ip) = " << sizeof(ip) << std::endl;
    std::cout << "sizeof(iphdr) = " << sizeof(iphdr) << std::endl;
    std::cout << "sizeof(ip6_hdr) = " << sizeof(ip6_hdr) << std::endl;
    std::cout << "sizeof(udphdr) = " << sizeof(udphdr) << std::endl;
}

#ifdef __CYGWIN__
// ----------------------------------------------------------------------------
int test_common_cygwin(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_common_cygwin" << std::endl;
    std::cout << "==============================================" << std::endl;

    int failure_counter = 0;




    return (failure_counter > 0) ? 1 : 0;
}
#endif

// ----------------------------------------------------------------------------
int test_common_binary(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_common_binary" << std::endl;
    std::cout << "==============================================" << std::endl;

    std::string
        one_byte("00001000"),
        two_bytes(one_byte + one_byte),
        four_bytes(two_bytes + two_bytes),
        eight_bytes(four_bytes + four_bytes);

    int failure_counter = 0;

    int8_t bits_8 = vdb::binary::parse_int8(one_byte);
    uint8_t bits_u8 = vdb::binary::parse_int8(one_byte);
    int16_t bits_16 = vdb::binary::parse_int16(two_bytes);
    uint16_t bits_u16 = vdb::binary::parse_int16(two_bytes);
    int32_t bits_32 = vdb::binary::parse_int32(four_bytes);
    uint32_t bits_u32 = vdb::binary::parse_int32(four_bytes);
    int64_t bits_64 = vdb::binary::parse_int64(eight_bytes);
    uint64_t bits_u64 = vdb::binary::parse_int64(eight_bytes);

    std::cout << "bits_8:   " << vdb::binary::str(bits_8) << std::endl;
    std::cout << "bits_u8:  " << vdb::binary::str(bits_u8) << std::endl;
    std::cout << "bits_16:  " << vdb::binary::str(bits_16) << std::endl;
    std::cout << "bits_u16: " << vdb::binary::str(bits_u16) << std::endl;
    std::cout << "bits_32:  " << vdb::binary::str(bits_32) << std::endl;
    std::cout << "bits_u32: " << vdb::binary::str(bits_u32) << std::endl;
    std::cout << "bits_64:  " << vdb::binary::str(bits_64) << std::endl;
    std::cout << "bits_u64: " << vdb::binary::str(bits_u64) << std::endl;

    ASSERT_TRUE(A0, vdb::binary::get_1_bit_8(0, bits_8) == 0);
    ASSERT_TRUE(A1, vdb::binary::get_1_bit_8(3, bits_8) == 1);
    ASSERT_TRUE(A2, vdb::binary::get_1_bit_8(3, bits_u8) == 1);
    ASSERT_TRUE(A3, vdb::binary::get_1_bit_8(4, bits_8) == 0);
    ASSERT_TRUE(A4, vdb::binary::get_1_bit_8(4, bits_u8) == 0);

    bits_8 = vdb::binary::set_1_bit_8(4, bits_8, 1);
    bits_u8 = vdb::binary::set_1_bit_8(4, bits_u8, 1);

    ASSERT_TRUE(B1, vdb::binary::get_1_bit_8(4, bits_8) == 1);
    ASSERT_TRUE(B2, vdb::binary::get_1_bit_8(4, bits_u8) == 1);

    bits_8 = vdb::binary::set_1_bit_8(4, bits_8, 0);
    bits_u8 = vdb::binary::set_1_bit_8(4, bits_u8, 0);

    ASSERT_TRUE(C1, vdb::binary::str(bits_8) == one_byte);
    ASSERT_TRUE(C2, vdb::binary::str(bits_u8) == one_byte);
    ASSERT_TRUE(C3, vdb::binary::str(bits_16) == two_bytes);
    ASSERT_TRUE(C4, vdb::binary::str(bits_u16) == two_bytes);
    ASSERT_TRUE(C5, vdb::binary::str(bits_32) == four_bytes);
    ASSERT_TRUE(C6, vdb::binary::str(bits_u32) == four_bytes);
    ASSERT_TRUE(C7, vdb::binary::str(bits_64) == eight_bytes);
    ASSERT_TRUE(C8, vdb::binary::str(bits_u64) == eight_bytes);

    ASSERT_TRUE(D1, vdb::binary::get_1_bit_16(0, 7) == 1);
    ASSERT_TRUE(D2, vdb::binary::get_1_bit_16(1, 7) == 1);
    ASSERT_TRUE(D3, vdb::binary::get_1_bit_16(2, 7) == 1);
    ASSERT_TRUE(D4, vdb::binary::get_1_bit_16(3, 7) == 0);
    ASSERT_TRUE(D5, vdb::binary::get_1_bit_16(4, 7) == 0);
    ASSERT_TRUE(D6, vdb::binary::get_1_bit_16(5, 7) == 0);

    bits_u32 = 0;

    std::cout << "bits_u32: " << vdb::binary::str(bits_u32) << std::endl;
    ASSERT_TRUE(E1, vdb::binary::get_1_bit_32(0, bits_u32) == 0);

    bits_u32 = 5;

    std::cout << "bits_u32: " << vdb::binary::str(bits_u32) << std::endl;
    ASSERT_TRUE(E2, vdb::binary::get_3_bits_32(2, bits_u32) == 5);

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_common_hexadecimal(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_common_hexadecimal" << std::endl;
    std::cout << "==============================================" << std::endl;

    std::string
        one_byte("DE"),
        two_bytes("DEAD"),
        four_bytes("DEADBEEF"),
        eight_bytes(four_bytes + four_bytes);

    int failure_counter = 0;

    uint8_t bits_u8 = vdb::hexadecimal::parse_int8(one_byte);
    uint16_t bits_u16 = vdb::hexadecimal::parse_int16(two_bytes);
    uint32_t bits_u32 = vdb::hexadecimal::parse_int32(four_bytes);
    uint64_t bits_u64 = vdb::hexadecimal::parse_int64(eight_bytes);

    ASSERT_TRUE(A1, bits_u8 == 0xDE);
    ASSERT_TRUE(A2, bits_u16 == 0xDEAD);
    ASSERT_TRUE(A3, bits_u32 == 0xDEADBEEF);
    ASSERT_TRUE(A4, bits_u64 == 0xDEADBEEFDEADBEEF);

    ASSERT_TRUE(B1, vdb::hexadecimal::str(bits_u8) == one_byte);
    ASSERT_TRUE(B2, vdb::hexadecimal::str(bits_u16) == two_bytes);
    ASSERT_TRUE(B3, vdb::hexadecimal::str(bits_u32) == four_bytes);
    ASSERT_TRUE(B4, vdb::hexadecimal::str(bits_u64) == eight_bytes);

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_common_logger(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_common_logger" << std::endl;
    std::cout << "==============================================" << std::endl;

    LOG_WARNING("Logging %s", "warning");
    LOG_ERROR("Logging %s", "error");

    return 0;
}

// ----------------------------------------------------------------------------
int test_common_string(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_common_string" << std::endl;
    std::cout << "==============================================" << std::endl;

    int failure_counter = 0;

    ASSERT_TRUE(A1, vdb::starts_with("ABCDEFG", "ABC"));
    ASSERT_FALSE(A2, vdb::starts_with("ABCDEFG", "EFG"));
    ASSERT_TRUE(A3, vdb::starts_with("ABCDEFG", "ABCDEFG"));
    ASSERT_FALSE(A4, vdb::starts_with("ABCDEFG", "ABCDEFGHIJK"));

    ASSERT_TRUE(B1, vdb::ends_with("ABCDEFG", "EFG"));
    ASSERT_FALSE(B2, vdb::ends_with("ABCDEFG", "ABC"));
    ASSERT_TRUE(B3, vdb::ends_with("ABCDEFG", "ABCDEFG"));
    ASSERT_FALSE(B4, vdb::ends_with("ABCDEFG", "ABCDEFGHIJK"));

    ASSERT_TRUE(C1, vdb::is_integer("1234"));
    ASSERT_TRUE(C2, vdb::is_integer("  1234  "));
    ASSERT_TRUE(C3, vdb::is_integer("-1234"));
    ASSERT_TRUE(C4, vdb::is_integer("  -1234  "));
    ASSERT_FALSE(C5, vdb::is_integer("  -  1234  "));
    ASSERT_FALSE(C6, vdb::is_integer("--1234"));
    ASSERT_FALSE(C7, vdb::is_integer("ABC"));

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_common_byte_stream(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_common_byte_stream" << std::endl;
    std::cout << "==============================================" << std::endl;

    vdb::byte_stream
        stream1(71);
    float32_t
        float32 = 3.1415927f,
        restored_float32 = 0;
    float64_t
        float64 = (float64_t)(float32 * float32),
        restored_float64 = 0;
    int8_t
        int8 = 0x32,
        restored_int8 = 0;
    uint8_t
        uint8 = 0xB2,
        restored_uint8 = 0;
    int16_t
        int16 = 0x3200,
        restored_int16 = 0;
    uint16_t
        uint16 = 0xB200,
        restored_uint16 = 0;
    int32_t
        int32 = 0x32000000,
        restored_int32 = 0;
    uint32_t
        uint32 = 0xB2000000,
        restored_uint32 = 0;
    int64_t
        int64 = 0x3200000000000000,
        restored_int64 = 0;
    uint64_t
        uint64 = 0xB200000000000000,
        restored_uint64 = 0;
    std::string
        string4 = "ABCD",
        restored_string4,
        string6 = "123456",
        restored_string6;
    uint8_t
        buffer1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
        restored_buffer1[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int
        failure_counter = 0;

    stream1.write(int8);
    stream1.write(uint8);
    stream1.write(int16);
    stream1.write(uint16);
    stream1.write(int32);
    stream1.write(uint32);
    stream1.write(int64);
    stream1.write(uint64);
    stream1.write(float32);
    stream1.write(float64);
    stream1.write(string4);
    stream1.write(string6);
    stream1.write(buffer1, 9);

    std::cout << "stream1.get_index() = "
              << stream1.get_index() << std::endl
              << "stream1.get_length() = "
              << stream1.get_length() << std::endl
              << "stream1.get_length_remaining() = "
              << stream1.get_length_remaining() << std::endl;

    vdb::hexadecimal::stream(
        stream1.get_buffer(),
        stream1.get_length(),
        16,
        std::cout);

    stream1.reset_index(0);

    stream1.read(restored_int8);
    stream1.read(restored_uint8);
    stream1.read(restored_int16);
    stream1.read(restored_uint16);
    stream1.read(restored_int32);
    stream1.read(restored_uint32);
    stream1.read(restored_int64);
    stream1.read(restored_uint64);
    stream1.read(restored_float32);
    stream1.read(restored_float64);
    stream1.read(restored_string4);
    stream1.read(restored_string6);
    stream1.read(restored_buffer1, 9);

    std::cout << "stream1.get_index() = "
              << stream1.get_index() << std::endl
              << "stream1.get_length() = "
              << stream1.get_length() << std::endl
              << "stream1.get_length_remaining() = "
              << stream1.get_length_remaining() << std::endl;

    ASSERT_TRUE(C1, restored_float32 == float32);
    ASSERT_TRUE(C2, restored_float64 == float64);
    ASSERT_TRUE(C3, restored_int8 == int8);
    ASSERT_TRUE(C4, restored_uint8 == uint8);
    ASSERT_TRUE(C5, restored_int16 == int16);
    ASSERT_TRUE(C6, restored_uint16 == uint16);
    ASSERT_TRUE(C7, restored_int32 == int32);
    ASSERT_TRUE(C8, restored_uint32 == uint32);
    ASSERT_TRUE(C9, restored_int64 == int64);
    ASSERT_TRUE(C10, restored_uint64 == uint64);
    ASSERT_TRUE(C11, restored_string4 == string4);
    ASSERT_TRUE(C12, restored_string6 == string6);

    vdb::hexadecimal::stream(
        restored_buffer1,
        9,
        16,
        std::cout);

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_common_bits(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_common_bits" << std::endl;
    std::cout << "==============================================" << std::endl;

    vdb::record8_t
        b1;
    vdb::record16_t
        b2;
    int
        failure_counter = 0;

    ASSERT_TRUE(A1, sizeof(vdb::record8_t) == 1);
    ASSERT_TRUE(A2, sizeof(b1) == 1);

    b1.value = (0x0D);

    std::cout << "b1.s1.v1 = " << (int)b1.s1.v1 << std::endl;
    std::cout << "b1.s1.v2 = " << (int)b1.s1.v2 << std::endl;
    std::cout << "b1.s1.v3 = " << (int)b1.s1.v3 << std::endl;
    std::cout << "b1.s1.v4 = " << (int)b1.s1.v4 << std::endl;
    std::cout << "b1.s1.v5 = " << (int)b1.s1.v5 << std::endl;
    std::cout << "b1.s1.v6 = " << (int)b1.s1.v6 << std::endl;
    std::cout << "b1.s1.v7 = " << (int)b1.s1.v7 << std::endl;
    std::cout << "b1.s1.v8 = " << (int)b1.s1.v8 << std::endl;
    std::cout << std::endl;
    std::cout << "b1.s2.v1 = " << (int)b1.s2.v1 << std::endl;
    std::cout << "b1.s2.v2 = " << (int)b1.s2.v2 << std::endl;
    std::cout << "b1.s2.v3 = " << (int)b1.s2.v3 << std::endl;
    std::cout << "b1.s2.v4 = " << (int)b1.s2.v4 << std::endl;
    std::cout << std::endl;
    std::cout << "b1.s4.v1 = " << (int)b1.s4.v1 << std::endl;
    std::cout << "b1.s4.v2 = " << (int)b1.s4.v2 << std::endl;

    ASSERT_TRUE(B1, sizeof(vdb::record16_t) == 2);
    ASSERT_TRUE(B2, sizeof(b2) == 2);

    b2.value = (0x0D0D);

    std::cout << "b2.s1.v1 = " << (int)b2.s1.v1 << std::endl;
    std::cout << "b2.s1.v2 = " << (int)b2.s1.v2 << std::endl;
    std::cout << "b2.s1.v3 = " << (int)b2.s1.v3 << std::endl;
    std::cout << "b2.s1.v4 = " << (int)b2.s1.v4 << std::endl;
    std::cout << "b2.s1.v5 = " << (int)b2.s1.v5 << std::endl;
    std::cout << "b2.s1.v6 = " << (int)b2.s1.v6 << std::endl;
    std::cout << "b2.s1.v7 = " << (int)b2.s1.v7 << std::endl;
    std::cout << "b2.s1.v8 = " << (int)b2.s1.v8 << std::endl;
    std::cout << "b2.s1.v9 = " << (int)b2.s1.v9 << std::endl;
    std::cout << "b2.s1.v10 = " << (int)b2.s1.v10 << std::endl;
    std::cout << "b2.s1.v11 = " << (int)b2.s1.v11 << std::endl;
    std::cout << "b2.s1.v12 = " << (int)b2.s1.v12 << std::endl;
    std::cout << "b2.s1.v13 = " << (int)b2.s1.v13 << std::endl;
    std::cout << "b2.s1.v14 = " << (int)b2.s1.v14 << std::endl;
    std::cout << "b2.s1.v15 = " << (int)b2.s1.v15 << std::endl;
    std::cout << "b2.s1.v16 = " << (int)b2.s1.v16 << std::endl;
    std::cout << std::endl;
    std::cout << "b2.s2.v1 = " << (int)b2.s2.v1 << std::endl;
    std::cout << "b2.s2.v2 = " << (int)b2.s2.v2 << std::endl;
    std::cout << "b2.s2.v3 = " << (int)b2.s2.v3 << std::endl;
    std::cout << "b2.s2.v4 = " << (int)b2.s2.v4 << std::endl;
    std::cout << "b2.s2.v5 = " << (int)b2.s2.v5 << std::endl;
    std::cout << "b2.s2.v6 = " << (int)b2.s2.v6 << std::endl;
    std::cout << "b2.s2.v7 = " << (int)b2.s2.v7 << std::endl;
    std::cout << "b2.s2.v8 = " << (int)b2.s2.v8 << std::endl;
    std::cout << std::endl;
    std::cout << "b2.s4.v1 = " << (int)b2.s4.v1 << std::endl;
    std::cout << "b2.s4.v2 = " << (int)b2.s4.v2 << std::endl;
    std::cout << "b2.s4.v3 = " << (int)b2.s4.v3 << std::endl;
    std::cout << "b2.s4.v4 = " << (int)b2.s4.v4 << std::endl;

    return (failure_counter > 0) ? 1 : 0;
}
