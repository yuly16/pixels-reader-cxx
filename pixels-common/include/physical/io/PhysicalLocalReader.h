//
// Created by liyu on 2/27/23.
//

#ifndef PIXELS_READER_PHYSICALLOCALREADER_H
#define PIXELS_READER_PHYSICALLOCALREADER_H

#include "physical/PhysicalReader.h"
#include "physical/storage/LocalFS.h"
#include "physical/natives/DirectRandomAccessFile.h"
#include <iostream>
#include <atomic>

class PhysicalLocalReader: public PhysicalReader {
public:
    PhysicalLocalReader(Storage * storage, std::string path);
    ByteBuffer * readFully(int length) override;
    void close();
    long getFileLength() override;
    void seek(long desired) override;
    long readLong() override;
    int readInt() override;
    char readChar() override;
    std::string getName() override;
private:
    LocalFS * local;
    std::string path;
    long id;
    std::atomic<int> numRequests;
    PixelsRandomAccessFile * raf;
};

#endif //PIXELS_READER_PHYSICALLOCALREADER_H
