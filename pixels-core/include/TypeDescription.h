//
// Created by liyu on 3/16/23.
//

#ifndef PIXELS_TYPEDESCRIPTION_H
#define PIXELS_TYPEDESCRIPTION_H

/**
 * TypeDescription derived from org.apache.orc
 * <p>
 * Schema description in a Pixels file.
 */

class TypeDescription {
public:
    static int SHORT_DECIMAL_MAX_PRECISION;
    static int LONG_DECIMAL_MAX_PRECISION;

    static int SHORT_DECIMAL_MAX_SCALE;
    static int LONG_DECIMAL_MAX_SCALE;

    static int SHORT_DECIMAL_DEFAULT_PRECISION;
    static int LONG_DECIMAL_DEFAULT_PRECISION;

    static int SHORT_DECIMAL_DEFAULT_SCALE;
    static int LONG_DECIMAL_DEFAULT_SCALE;

    static int DEFAULT_LENGTH;

    static int DEFAULT_CHAR_LENGTH;

    static int DEFAULT_TIMESTAMP_PRECISION;

    static int MAX_TIMESTAMP_PRECISION;
private:
    static long serialVersionUID;
};
#endif //PIXELS_TYPEDESCRIPTION_H
