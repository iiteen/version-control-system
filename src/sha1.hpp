#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>
#include <sstream>

using namespace std;
string sha1(const string &input)
{
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // Pre-processing: Padding the message
    std::string paddedMessage = input;
    uint64_t messageLength = input.length() * 8; // Length in bits
    paddedMessage += static_cast<char>(0x80);
    while ((paddedMessage.length() * 8) % 512 != 448)
    {
        paddedMessage += static_cast<char>(0x00);
    }
    for (int i = 56; i >= 0; i -= 8)
    {
        paddedMessage += static_cast<char>((messageLength >> i) & 0xFF);
    }

    // Process the message in 512-bit blocks
    for (size_t offset = 0; offset < paddedMessage.length(); offset += 64)
    {
        uint32_t words[80];
        for (int i = 0; i < 16; ++i)
        {
            words[i] = static_cast<uint32_t>(
                paddedMessage[offset + i * 4] << 24 |
                paddedMessage[offset + i * 4 + 1] << 16 |
                paddedMessage[offset + i * 4 + 2] << 8 |
                paddedMessage[offset + i * 4 + 3]);
        }
        for (int i = 16; i < 80; ++i)
        {
            words[i] = (words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16]);
            words[i] = (words[i] << 1) | (words[i] >> 31);
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        for (int i = 0; i < 80; ++i)
        {
            uint32_t f, k;
            if (i < 20)
            {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (i < 40)
            {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (i < 60)
            {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else
            {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = (a << 5) | (a >> 27);
            temp += f + e + k + words[i];
            e = d;
            d = c;
            c = (b << 30) | (b >> 2);
            b = a;
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // Concatenate the hash components
    uint32_t hashComponents[5] = {h0, h1, h2, h3, h4};
    std::string result;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 24; j >= 0; j -= 8)
        {
            result += static_cast<char>((hashComponents[i] >> j) & 0xFF);
        }
    }

    return result;
}


string sha1_hash(string input)
{
    // string input = "Hello, World!";
    string hash = sha1(input);
    ostringstream hexStream;

    for (char c : hash)
    {
        hexStream << hex << setw(2) << setfill('0') << static_cast<int>(c & 0xFF);
    }
    return hexStream.str();
}
