//
// Created by liyu on 3/16/23.
//

#include "TypeDescription.h"

long TypeDescription::serialVersionUID = 4270695889340023552L;

/**
 * In SQL standard, the max precision of decimal is 38.
 * Issue #196: support short decimal of which the max precision is 18.
 * Issue #203: support long decimal of which the max precision is 38.
 */
int TypeDescription::SHORT_DECIMAL_MAX_PRECISION = 18;
int TypeDescription::LONG_DECIMAL_MAX_PRECISION = 38;

/**
 * In SQL standard, the max scale of decimal is 38.
 * Issue #196: support short decimal of which the max scale is 18.
 * Issue #203: support long decimal of which the max scale is 38.
 */
int TypeDescription::SHORT_DECIMAL_MAX_SCALE = 18;
int TypeDescription::LONG_DECIMAL_MAX_SCALE = 38;
/**
 * In SQL standard, the default precision of decimal is 38.
 * Issue #196: support short decimal of which the default precision is 18.
 * Issue #203: support long decimal of which the default precision is 38.
 */
int TypeDescription::SHORT_DECIMAL_DEFAULT_PRECISION = 18;
int TypeDescription::LONG_DECIMAL_DEFAULT_PRECISION = 38;
/**
 * It is a standard that the default scale of decimal is 0.
 */
int TypeDescription::SHORT_DECIMAL_DEFAULT_SCALE = 0;
int TypeDescription::LONG_DECIMAL_DEFAULT_SCALE = 0;
/**
 * The default length of varchar, binary, and varbinary.
 */
int TypeDescription::DEFAULT_LENGTH = 65535;
/**
 * It is a standard that the default length of char is 1.
 */
int TypeDescription::DEFAULT_CHAR_LENGTH = 1;
/**
 * In SQL standard, the default precision of timestamp is 6 (i.e., microseconds),
 * however, in Pixels, we use default precision 3 to be compatible with Trino.
 */
int TypeDescription::DEFAULT_TIMESTAMP_PRECISION = 3;
/**
 * 9 = nanosecond, 6 = microsecond, 3 = millisecond, 0 = second.
 * <p>Although 64-bit long is enough to encode the nanoseconds for now, it is
 * risky to encode a nanosecond time in the near future, thus some query engines
 * such as Trino only use long to encode a timestamp up to 6 precision.
 * Therefore, we also set the max precision of long encoded timestamp to 6 in Pixels.</p>
 */
int TypeDescription::MAX_TIMESTAMP_PRECISION = 6;

