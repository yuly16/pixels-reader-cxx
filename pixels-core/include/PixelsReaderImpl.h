//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PIXELSREADERIMPL_H
#define PIXELS_PIXELSREADERIMPL_H

#include "PixelsReader.h"
#include "reader/PixelsRecordReaderImpl.h"
#include <iostream>
#include <vector>
class PixelsReaderBuilder;

class PixelsReaderImpl: public PixelsReader {
public:
    PixelsRecordReader * read();
    friend class PixelsReaderBuilder;
private:
    std::vector<PixelsRecordReader *> recordReaders;
    PixelsReaderImpl(PhysicalReader * reader);
    PhysicalReader * physicalReader;
};

#endif //PIXELS_PIXELSREADERIMPL_H
