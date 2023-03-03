//
// Created by yuliangyong on 2023-03-02.
//

#ifndef PIXELS_PIXELSRANDOMACCESSFILE_H
#define PIXELS_PIXELSRANDOMACCESSFILE_H

#include <iostream>
class PixelsRandomAccessFile {
public:
    virtual void seek(long offset) = 0;
    virtual long length() = 0;
    virtual char * readFully(int len) = 0;
    virtual std::string readLine();
    virtual std::string readUTF();

};

#endif //PIXELS_PIXELSRANDOMACCESSFILE_H
