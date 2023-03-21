//
// Created by liyu on 3/21/23.
//

#include "utils/EncodingUtils.h"

int EncodingUtils::decodeBitWidth(int n) {
    if (n >= FixedBitSizes::ONE && n <= FixedBitSizes::TWENTYFOUR) {
        return n + 1;
    }
    else if (n == FixedBitSizes::TWENTYSIX) {
        return 26;
    }
    else if (n == FixedBitSizes::TWENTYEIGHT) {
        return 28;
    }
    else if (n == FixedBitSizes::THIRTY) {
        return 30;
    }
    else if (n == FixedBitSizes::THIRTYTWO) {
        return 32;
    }
    else if (n == FixedBitSizes::FORTY) {
        return 40;
    }
    else if (n == FixedBitSizes::FORTYEIGHT) {
        return 48;
    }
    else if (n == FixedBitSizes::FIFTYSIX) {
        return 56;
    }
    else {
        return 64;
    }
}

void EncodingUtils::unrolledUnPack4(long *buffer, int offset, int len,
                                    const std::shared_ptr<ByteBuffer> &input) {
    int numHops = 2;
    int remainder = len % numHops;
    int endOffset = offset + len;
    int endUnroll = endOffset - remainder;
    int val = 0;
    for (int i = offset; i < endUnroll; i = i + numHops)
    {
        val = input->get();
        buffer[i] = ((uint32_t)val >> 4) & 15;
        buffer[i + 1] = val & 15;
    }

    if (remainder > 0)
    {
        int startShift = 4;
        val = input->get();
        for (int i = endUnroll; i < endOffset; i++)
        {
            buffer[i] = ((uint32_t)val >> startShift) & 15;
            startShift -= 4;
        }
    }
}
