//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PIXELSREADERBUILDER_H
#define PIXELS_PIXELSREADERBUILDER_H

#include "PixelsReaderImpl.h"

class PixelsReaderBuilder {
public:
    PixelsReaderBuilder();
    PixelsReaderBuilder * setStorage(Storage * storage);
    PixelsReader * build();
private:
    Storage * buildStorage;
};
#endif //PIXELS_PIXELSREADERBUILDER_H
