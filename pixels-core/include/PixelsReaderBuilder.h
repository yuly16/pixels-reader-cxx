//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PIXELSREADERBUILDER_H
#define PIXELS_PIXELSREADERBUILDER_H

#include "PixelsReaderImpl.h"
#include "physical/PhysicalReaderUtil.h"
#include "pixels-common/pixels.pb.h"
#include "PixelsVersion.h"

class PixelsReaderBuilder {
public:
    PixelsReaderBuilder();
    PixelsReaderBuilder * setStorage(Storage * storage);
    PixelsReaderBuilder * setPath(const std::string & path);
    PixelsReader * build();
private:
    Storage * builderStorage;
    std::string builderPath;
};
#endif //PIXELS_PIXELSREADERBUILDER_H
