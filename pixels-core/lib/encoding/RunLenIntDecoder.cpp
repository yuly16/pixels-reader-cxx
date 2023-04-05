//
// Created by liyu on 3/20/23.
//

#include "encoding/RunLenIntDecoder.h"

RunLenIntDecoder::RunLenIntDecoder(const std::shared_ptr <ByteBuffer>& bb, bool isSigned) {
    literals = new long[Constants::MAX_SCOPE];
    inputStream = bb;
    this->isSigned = isSigned;
    numLiterals = 0;
    used = 0;
}

void RunLenIntDecoder::close() {

}

RunLenIntDecoder::~RunLenIntDecoder() {
	if(literals != nullptr) {
		delete[] literals;
		literals = nullptr;
	}
}

long RunLenIntDecoder::next() {
    long result;
    if(used == numLiterals) {
        numLiterals = 0;
        used = 0;
        readValues();
    }
    result = literals[used++];
    return result;
}

void RunLenIntDecoder::readValues() {
    auto firstByte = inputStream->get();
    if(firstByte < 0) {
        // TODO: logger.error
        used = 0;
        numLiterals = 0;
        return;
    }
    // here we do unsigned shift
    auto currentEncoding = (EncodingType) (((uint8_t)firstByte >> 6) & 0x03);
    switch (currentEncoding) {
        case RunLenIntEncoder::SHORT_REPEAT:
            throw InvalidArgumentException("Currently "
                                           "we don't support SHORT_REPEAT encoding.");
        case RunLenIntEncoder::DIRECT:
            readDirectValues(firstByte);
            break;
        case RunLenIntEncoder::PATCHED_BASE:
            throw InvalidArgumentException("Currently "
                                           "we don't support PATCHED_BASE encoding.");
        case RunLenIntEncoder::DELTA:
            throw InvalidArgumentException("Currently "
                                           "we don't support DELTA encoding.");
        default:
            throw InvalidArgumentException("Not supported encoding type.");
    }
}

void RunLenIntDecoder::readDirectValues(int8_t firstByte) {
    // extract the number of fixed bits;
    uint8_t fbo = ((uint8_t)firstByte >> 1) & 0x1f;
    int fb = encodingUtils.decodeBitWidth(fbo);

    // extract run length
    int len = (firstByte & 0x01) << 8;
    len |= inputStream->get();
    // runs are one off
    len += 1;

    // write the unpacked value and zigzag decode to result buffer
    readInts(literals, numLiterals, len, fb, inputStream);
    if(isSigned) {
        for (int i = 0; i < len; i++) {
            literals[numLiterals] = zigzagDecode(literals[numLiterals]);
            numLiterals++;
        }
    } else {
        numLiterals += len;
    }
}

long RunLenIntDecoder::zigzagDecode(long val) {
    return (long) (((uint64_t)val >> 1) ^ -(val & 1));
}

/**
 * Read bitpacked integers from input stream
 */
void RunLenIntDecoder::readInts(long *buffer, int offset, int len, int bitSize,
                           const std::shared_ptr<ByteBuffer> &input) {
    int bitsLeft = 0;
    int current = 0;
    switch (bitSize) {
        case 4:
            encodingUtils.unrolledUnPack4(buffer, offset, len, input);
            return;
        case 8:
            encodingUtils.unrolledUnPack8(buffer, offset, len, input);
            return;
        default:
            throw InvalidArgumentException("RunLenIntDecoder::readInts: "
                                           "not supported bitSize.");
    }
    // TODO: if notthe case, we should write the following code.
}


