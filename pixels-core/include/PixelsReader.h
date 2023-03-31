//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PIXELSREADER_H
#define PIXELS_PIXELSREADER_H

/**
 * Pixels file reader.
 * This interface is for reading pixels content as
 * {@link VectorizedRowBatch}.
 *
 * @author liangyong
 */

#include "physical/storage/LocalFS.h"
#include "physical/PhysicalReader.h"
#include "reader/PixelsRecordReader.h"
#include "reader/PixelsReaderOption.h"

class PixelsReader {
public:
    /**
     * Get a <code>PixelsRecordReader</code>
     *
     * @return record reader
     */
    virtual std::shared_ptr<PixelsRecordReader> read(PixelsReaderOption option) = 0;
};

#endif //PIXELS_PIXELSREADER_H
