//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PIXELSREADERIMPL_H
#define PIXELS_PIXELSREADERIMPL_H

#include "PixelsReader.h"
#include "reader/PixelsRecordReaderImpl.h"
#include <iostream>
#include <vector>
#include "pixels-common/pixels.pb.h"
#include "PixelsFooterCache.h"

class PixelsReaderBuilder;

class PixelsReaderImpl: public PixelsReader {
public:
    PixelsRecordReader * read();
    friend class PixelsReaderBuilder;
private:
    std::vector<PixelsRecordReader *> recordReaders;
    PixelsReaderImpl(PhysicalReader *reader,
                     const pixels::proto::FileTail& fileTail,
                     const PixelsFooterCache& footerCache);
    PhysicalReader * physicalReader;
    PixelsFooterCache pixelsFooterCache;
    pixels::proto::PostScript postScript;
    pixels::proto::Footer footer;
};

#endif //PIXELS_PIXELSREADERIMPL_H
