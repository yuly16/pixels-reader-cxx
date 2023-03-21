//
// Created by liyu on 3/19/23.
//

#ifndef PIXELS_VARCHARCOLUMNREADER_H
#define PIXELS_VARCHARCOLUMNREADER_H

#include "reader/StringColumnReader.h"
#include "reader/ColumnReader.h"
class VarcharColumnReader: public ColumnReader {
public:
    explicit VarcharColumnReader(std::shared_ptr<TypeDescription> type);
    void close() override;
    void read(std::shared_ptr<ByteBuffer> input,
              pixels::proto::ColumnEncoding encoding,
              int offset, int size, int pixelStride,
              int vectorIndex, std::shared_ptr<ColumnVector> vector,
              pixels::proto::ColumnChunkIndex chunkIndex) override;
};
#endif //PIXELS_VARCHARCOLUMNREADER_H
