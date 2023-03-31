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
#include "reader/PixelsReaderOption.h"


class PixelsReaderBuilder;

class PixelsReaderImpl: public PixelsReader {
public:
    std::shared_ptr<PixelsRecordReader> read(PixelsReaderOption option);
	PixelsReaderImpl(std::shared_ptr<PhysicalReader> reader,
	                 const pixels::proto::FileTail& fileTail,
	                 const PixelsFooterCache& footerCache);
private:
    std::vector<std::shared_ptr<PixelsRecordReader>> recordReaders;

    std::shared_ptr<PhysicalReader> physicalReader;
    PixelsFooterCache pixelsFooterCache;
    pixels::proto::PostScript postScript;
    pixels::proto::Footer footer;
};

#endif //PIXELS_PIXELSREADERIMPL_H
