//
// Created by liyu on 3/6/23.
//

#include "PixelsReaderImpl.h"


PixelsReaderImpl::PixelsReaderImpl(PhysicalReader * reader) {
    physicalReader = reader;
}

/**
 * Prepare for the next row batch. This method is independent from readBatch().
 *
 * @param batchSize the willing batch size
 * @return the real batch size
 */
PixelsRecordReader *PixelsReaderImpl::read() {
    // TODO: add a function parameter, and the code before creating PixelsRecordReaderImpl
    PixelsRecordReader * recordReader = new PixelsRecordReaderImpl(physicalReader);
    recordReaders.push_back(recordReader);
    return recordReader;
}
