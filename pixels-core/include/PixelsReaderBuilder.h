//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PIXELSREADERBUILDER_H
#define PIXELS_PIXELSREADERBUILDER_H

#include "PixelsReaderImpl.h"
#include "physical/PhysicalReaderUtil.h"
#include "pixels-common/pixels.pb.h"
#include "PixelsVersion.h"
#include "PixelsFooterCache.h"
#include "exception/PixelsReaderException.h"
#include "exception/InvalidArgumentException.h"
#include "exception/PixelsFileVersionInvalidException.h"
#include "exception/PixelsFileMagicInvalidException.h"
#include "utils/Constants.h"

class PixelsReaderBuilder {
public:
    PixelsReaderBuilder();
    PixelsReaderBuilder * setStorage(Storage * storage);
    PixelsReaderBuilder * setPath(const std::string & path);
    PixelsReaderBuilder * setPixelsFooterCache(const PixelsFooterCache &pixelsFooterCache);
    PixelsReader * build();

private:
    Storage * builderStorage;
    std::string builderPath;
    PixelsFooterCache builderPixelsFooterCache;
};
#endif //PIXELS_PIXELSREADERBUILDER_H
