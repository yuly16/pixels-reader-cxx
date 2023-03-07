//
// Created by liyu on 3/7/23.
//

#ifndef PIXELS_PIXELSRECORDREADER_H
#define PIXELS_PIXELSRECORDREADER_H

class PixelsRecordReader {
    /**
     * Prepare for the next row batch. This method is independent from readBatch().
     *
     * @param batchSize the willing batch size
     * @return the real batch size
     */
//    virtual int prepareBatch(int batchSize) = 0;

    /**
     * Read the next row batch. This method is thread-safe and independent from prepareBatch().
     *
     * @param batchSize the row batch size
     * @return vectorized row batch
     */


};
#endif //PIXELS_PIXELSRECORDREADER_H
