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
    PhysicalLocalReader(std::shared_ptr<Storage> storage, std::string path);
    std::shared_ptr<ByteBuffer> readFully(int length) override;
	void readAsync(int length, int asyncRequestId = 0) override;
	std::pair<int, std::shared_ptr<ByteBuffer>> completeAsync() override;
    void close() override;
    long getFileLength() override;
    void seek(long desired) override;
    long readLong() override;
    int readInt() override;
    char readChar() override;
    std::string getName() override;
private:
    std::shared_ptr<LocalFS> local;
    std::string path;
    long id;
    std::atomic<int> numRequests;
	std::atomic<int> asyncNumRequests;
	std::shared_ptr<PixelsRandomAccessFile> raf;

};

#endif //PIXELS_READER_PHYSICALLOCALREADER_H
