//
// Created by yuliangyong on 2023-03-02.
//

#ifndef PIXELS_DIRECTRANDOMACCESSFILE_H
#define PIXELS_DIRECTRANDOMACCESSFILE_H

#include "physical/natives/PixelsRandomAccessFile.h"
#include "physical/natives/ByteBuffer.h"
#include "physical/natives/DirectIoLib.h"
#include <fcntl.h>
#include <unistd.h>
#include "liburing.h"
#include "liburing/io_uring.h"
#include "utils/TimeProfiler.h"

class DirectRandomAccessFile: public PixelsRandomAccessFile {
public:
    explicit DirectRandomAccessFile(const std::string& file);
    void close() override;
    std::shared_ptr<ByteBuffer> readFully(int len) override;
	void readAsync(int len, int uringRequestId = 0);
	// must be called after readAsync
	std::pair<int, std::shared_ptr<ByteBuffer>>  completeAsync();
    long length() override;
    void seek(long off) override;
    long readLong() override;
    char readChar() override;
    int readInt() override;
    ~DirectRandomAccessFile();
private:
    void populatedBuffer();
    std::vector<std::shared_ptr<ByteBuffer>> largeBuffers;
	/* smallDirectBuffer align to blockSize. smallBuffer adds the offset to smallDirectBuffer. */
    std::shared_ptr<ByteBuffer> smallBuffer;
	std::shared_ptr<ByteBuffer> smallDirectBuffer;
	std::shared_ptr<DirectIoLib> directIoLib;
	// this is for async io
	struct io_uring ring;
    bool bufferValid;
    long offset;
    long len;
    int fd;
	int fsBlockSize;


};
#endif //PIXELS_DIRECTRANDOMACCESSFILE_H
