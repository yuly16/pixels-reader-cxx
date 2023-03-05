//
// Created by yuliangyong on 2023-03-02.
//

#ifndef PIXELS_DIRECTRANDOMACCESSFILE_H
#define PIXELS_DIRECTRANDOMACCESSFILE_H

#include "physical/natives/PixelsRandomAccessFile.h"
#include "physical/natives/ByteBuffer.h"
#include <fcntl.h>
#include <unistd.h>

class DirectRandomAccessFile: public PixelsRandomAccessFile {
public:
    explicit DirectRandomAccessFile(const std::string& file);
    void close() override;
    ByteBuffer * readFully(int len) override;
    ~DirectRandomAccessFile();
private:
    std::vector<ByteBuffer *> largeBuffers;
    ByteBuffer * smallBuffer;
    bool bufferValid;
    int blockSize;
    long offset;
    long length;
    int fd;


};
#endif //PIXELS_DIRECTRANDOMACCESSFILE_H
