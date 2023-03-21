//
// Created by liyu on 3/21/23.
//

#ifndef PIXELS_ENCODINGUTILS_H
#define PIXELS_ENCODINGUTILS_H

#include <memory>
#include "physical/natives/ByteBuffer.h"

class EncodingUtils {
public:
    EncodingUtils() = default;
    int decodeBitWidth(int n);

    void unrolledUnPack4(long *buffer, int offset, int len,
                         const std::shared_ptr<ByteBuffer> &input);

private:
enum FixedBitSizes{
        ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, ELEVEN, TWELVE,
        THIRTEEN, FOURTEEN, FIFTEEN, SIXTEEN, SEVENTEEN, EIGHTEEN, NINETEEN,
        TWENTY, TWENTYONE, TWENTYTWO, TWENTYTHREE, TWENTYFOUR, TWENTYSIX,
        TWENTYEIGHT, THIRTY, THIRTYTWO, FORTY, FORTYEIGHT, FIFTYSIX, SIXTYFOUR
    };

};
#endif //PIXELS_ENCODINGUTILS_H
