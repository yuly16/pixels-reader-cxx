//
// Created by liyu on 3/6/23.
//

#include "PixelsReaderImpl.h"

PixelsReaderImpl::PixelsReaderImpl(PhysicalReader * reader) {
    physicalReader = reader;
}

PixelsRecordReader *PixelsReaderImpl::read() {
    // TODO: add a function parameter, and the code before creating PixelsRecordReaderImpl
    PixelsRecordReader * recordReader = new PixelsRecordReaderImpl();
    recordReaders.push_back(recordReader);
    return recordReader;
}
