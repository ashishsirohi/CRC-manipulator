#include "CRC/CRC32.h"

/**
 * NOTICE: following code is strongly based on SAR-PR-2006-05
 */

CRCType CRC32::makeNextChecksum(CRCType prevChecksum, uint8_t c) const
{
    uint8_t tmp = prevChecksum ^ c;
    return static_cast<uint32_t>((prevChecksum >> 8) ^ lookupTable[tmp]);
}

CRCType CRC32::makePrevChecksum(CRCType nextChecksum, uint8_t c) const
{
    uint8_t tmp = nextChecksum >> 24;
    return static_cast<uint32_t>((nextChecksum << 8) ^ invLookupTable[tmp] ^ c);
}

CRC32::CRC32() : CRC(4, 0x04C11DB7, 0xffffffff, 0xffffffff)
{
    for (unsigned short n = 0; n <= 0xff; n++)
    {
        uint32_t crc1 = n;
        uint32_t crc2 = n << 24;
        for (unsigned char k = 0; k < 8; k++)
        {
            if (crc1 & 1)
                crc1 = (crc1 >> 1) ^ polynomialReverse;
            else
                crc1 >>= 1;

            if (crc2 & 0x80000000)
                crc2 = ((crc2 ^ polynomialReverse) << 1) | 1;
            else
                crc2 <<= 1;
        }
        lookupTable[n] = crc1;
        invLookupTable[n] = crc2;
    }
}
