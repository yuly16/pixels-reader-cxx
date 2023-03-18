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

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <pixels-common/pixels.pb.h>
#include <vector/VectorizedRowBatch.h>
#include "vector/LongColumnVector.h"
#include "vector/ByteColumnVector.h"
#include "vector/BinaryColumnVector.h"

struct CategoryProperty {
    bool isPrimitive;
    std::vector<std::string> names;
};

class TypeDescription {
public:
    enum Category {
        BOOLEAN,
        BYTE,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
        DECIMAL,
        STRING,
        DATE,
        TIME,
        TIMESTAMP,
        VARBINARY,
        BINARY,
        VARCHAR,
        CHAR,
        STRUCT
    };
    TypeDescription() = default;
    TypeDescription(Category c);
    static TypeDescription createBoolean();
    static TypeDescription createByte();
    static TypeDescription createShort();
    static TypeDescription createInt();
    static TypeDescription createLong();
    static TypeDescription createFloat();
    static TypeDescription createDouble();
    static TypeDescription createString();
    static TypeDescription createDate();
    static TypeDescription createTime();
    static TypeDescription createTimestamp();
    static TypeDescription createVarbinary();
    static TypeDescription createBinary();
    static TypeDescription createVarchar();
    static TypeDescription createChar();
    static TypeDescription createStruct();
    static TypeDescription createSchema(const std::vector<pixels::proto::Type>& types);
    std::shared_ptr<TypeDescription> addField(const std::string& field, const std::shared_ptr<TypeDescription>& fieldType);
    void setParent(const std::shared_ptr<TypeDescription>& p);
    std::shared_ptr<VectorizedRowBatch> createRowBatch(int maxSize, const std::vector<bool>& useEncodedVector);
    std::vector<std::shared_ptr<TypeDescription>> getChildren();
    Category getCategory();
    static std::map<Category, CategoryProperty> categoryMap;

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
    std::shared_ptr<ColumnVector> createColumn(int maxSize, bool useEncodedVector);
    std::shared_ptr<ColumnVector> createColumn(int maxSize, std::vector<bool> useEncodedVector);
    static long serialVersionUID;
    int id;
    int maxId;
    std::shared_ptr<TypeDescription> parent;
    Category category;
    std::vector<std::shared_ptr<TypeDescription>> children;
    std::vector<std::string> fieldNames;
    uint32_t maxLength;
    uint32_t precision;
    uint32_t scale;
};
#endif //PIXELS_TYPEDESCRIPTION_H
