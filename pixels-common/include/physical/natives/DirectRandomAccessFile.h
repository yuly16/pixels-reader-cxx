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
    std::shared_ptr<ByteBuffer> readFully(int len) override;
    long length() override;
    void seek(long off) override;
    long readLong() override;
    char readChar() override;
    int readInt() override;
    ~DirectRandomAccessFile();
private:
    void populatedBuffer();
    std::vector<std::shared_ptr<ByteBuffer>> largeBuffers;
    ByteBuffer * smallBuffer;
    bool bufferValid;
    int blockSize;
    long offset;
    long len;
    int fd;


};
#endif //PIXELS_DIRECTRANDOMACCESSFILE_H
