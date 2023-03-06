//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PIXELSREADERIMPL_H
#define PIXELS_PIXELSREADERIMPL_H

#include "PixelsReader.h"

class PixelsReaderBuilder;

class PixelsReaderImpl: public PixelsReader {
public:
    friend class PixelsReaderBuilder;
private:
    PixelsReaderImpl(PhysicalReader * reader);
    PhysicalReader * physicalReader;
};

#endif //PIXELS_PIXELSREADERIMPL_H
