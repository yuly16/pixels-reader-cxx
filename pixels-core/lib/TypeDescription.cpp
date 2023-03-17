//
// Created by liyu on 3/16/23.
//

#include "TypeDescription.h"

#include <utility>
#include "exception/InvalidArgumentException.h"

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


std::map<TypeDescription::Category, CategoryProperty> TypeDescription::categoryMap = {
        {BOOLEAN, {true, {"boolean"}}},
        {BYTE, {true, {"tinyint", "byte"}}},
        {SHORT, {true, {"smallint", "short"}}},
        {INT, {true, {"integer", "int"}}},
        {LONG, {true, {"bigint", "long"}}},
        {FLOAT, {true, {"float", "real"}}},
        {DOUBLE, {true, {"double"}}},
        {DECIMAL, {true, {"decimal"}}},
        {STRING, {true, {"string"}}},
        {DATE, {true, {"date"}}},
        {TIME, {true, {"time"}}},
        {TIMESTAMP, {true, {"timestamp"}}},
        {VARBINARY, {true, {"varbinary"}}},
        {BINARY, {true, {"binary"}}},
        {VARCHAR, {true, {"varchar"}}},
        {CHAR, {true, {"char"}}},
        {STRUCT, {false, {"struct"}}}
};

TypeDescription::TypeDescription(Category c) {
    id = -1;
    maxId = -1;
    maxLength = DEFAULT_LENGTH;
    precision = SHORT_DECIMAL_DEFAULT_PRECISION;
    scale = SHORT_DECIMAL_DEFAULT_SCALE;

    category = c;
}

TypeDescription TypeDescription::createSchema(const std::vector<pixels::proto::Type>& types) {
    TypeDescription schema(createStruct());
    for(const auto& type : types) {
        const std::string& fieldName = type.name();
        TypeDescription fieldType;
        switch (type.kind()) {
            case pixels::proto::Type_Kind_BOOLEAN:
                fieldType = TypeDescription::createBoolean();
                break;
            case pixels::proto::Type_Kind_LONG:
                fieldType = TypeDescription::createLong();
                break;
            case pixels::proto::Type_Kind_INT:
                fieldType = TypeDescription::createInt();
                break;
            case pixels::proto::Type_Kind_SHORT:
                fieldType = TypeDescription::createShort();
                break;
            case pixels::proto::Type_Kind_BYTE:
                fieldType = TypeDescription::createByte();
                break;
            case pixels::proto::Type_Kind_FLOAT:
                fieldType = TypeDescription::createFloat();
                break;
            case pixels::proto::Type_Kind_DOUBLE:
                fieldType = TypeDescription::createDouble();
                break;
            case pixels::proto::Type_Kind_STRING:
                fieldType = TypeDescription::createString();
                break;
            case pixels::proto::Type_Kind_DATE:
                fieldType = TypeDescription::createDate();
                break;
            case pixels::proto::Type_Kind_TIME:
                fieldType = TypeDescription::createTime();
                break;
            default:
                throw InvalidArgumentException("Unknown type: " + type.name());
        }
        schema.addField(fieldName, std::make_shared<TypeDescription>(fieldType));
    }
    return schema;
}

TypeDescription TypeDescription::createBoolean() {
    return {BOOLEAN};
}

TypeDescription TypeDescription::createByte() {
    return {BYTE};
}

TypeDescription TypeDescription::createShort() {
    return {SHORT};
}

TypeDescription TypeDescription::createInt() {
    return {INT};
}

TypeDescription TypeDescription::createLong() {
    return {LONG};
}

TypeDescription TypeDescription::createFloat() {
    return {FLOAT};
}

TypeDescription TypeDescription::createDouble() {
    return {DOUBLE};
}


TypeDescription TypeDescription::createString() {
    return {STRING};
}

TypeDescription TypeDescription::createDate() {
    return {DATE};
}

TypeDescription TypeDescription::createTime() {
    return {TIME};
}

TypeDescription TypeDescription::createTimestamp() {
    return {TIMESTAMP};
}

TypeDescription TypeDescription::createVarbinary() {
    return {VARBINARY};
}

TypeDescription TypeDescription::createBinary() {
    return {BINARY};
}

TypeDescription TypeDescription::createVarchar() {
    return {VARCHAR};
}

TypeDescription TypeDescription::createChar() {
    return {CHAR};
}

TypeDescription TypeDescription::createStruct() {
    return {STRUCT};
}

std::shared_ptr<TypeDescription> TypeDescription::addField(const std::string& field, const std::shared_ptr<TypeDescription>& fieldType) {
    if(category != STRUCT) {
        throw InvalidArgumentException("Can only add fields to struct type,"
                                       "but not " + categoryMap[category].names[0]);
    }
    fieldNames.emplace_back(field);
    children.emplace_back(fieldType);
    std::shared_ptr<TypeDescription> this_ptr(this);
    fieldType->setParent(this_ptr);
    return this_ptr;
}

void TypeDescription::setParent(const std::shared_ptr<TypeDescription>& p) {
    parent = p;
}

