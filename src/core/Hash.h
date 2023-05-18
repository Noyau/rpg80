#pragma once

using hash_t = uint32_t;

// Hash Lookup table
template<hash_t X, uint8_t N = 8>
struct Hash : Hash<((X & 1) ? 0xedb88320 : 0) ^ (X >> 1), N - 1>
{};

template<hash_t X>
struct Hash<X, 0>
{
    enum : hash_t { Value = X };
};

#define HASH_A(x)   HASH_B(x) HASH_B((x) + 128)
#define HASH_B(x)   HASH_C(x) HASH_C((x) + 64)
#define HASH_C(x)   HASH_D(x) HASH_D((x) + 32)
#define HASH_D(x)   HASH_E(x) HASH_E((x) + 16)
#define HASH_E(x)   HASH_F(x) HASH_F((x) + 8)
#define HASH_F(x)   HASH_G(x) HASH_G((x) + 4)
#define HASH_G(x)   HASH_H(x) HASH_H((x) + 2)
#define HASH_H(x)   HASH_I(x) HASH_I((x) + 1)
#define HASH_I(x)   Hash<x>::Value ,

constexpr hash_t crcTable[]{ HASH_A(0) };

// Constexpr implementation and helpers
constexpr size_t Crc32Length(const char* str)
{
    if (str == nullptr || *str == '\0')
        return 0;

    return 1 + Crc32Length(str + 1);
}

constexpr hash_t Crc32(const char* p, size_t len, hash_t crc)
{
    if (len < 1)
        return crc;

    const size_t index{ (crc & 0xff) ^ *p };
    const hash_t hash{ crcTable[index] };

    return Crc32(p + 1, len - 1, (crc >> 8) ^ hash);
}

constexpr hash_t Crc32(const char* data, size_t len)
{
    return ~Crc32(data, len, ~0);
}

constexpr hash_t Crc32(const char* str)
{
    return Crc32(str, Crc32Length(str));
}

#undef HASH_I
#undef HASH_H
#undef HASH_G
#undef HASH_F
#undef HASH_E
#undef HASH_D
#undef HASH_C
#undef HASH_B
#undef HASH_A