//
// Created by liyu on 3/6/23.
//

#include "PixelsReaderImpl.h"

PixelsReaderImpl::PixelsReaderImpl(std::shared_ptr<PhysicalReader> reader,
                                   const pixels::proto::FileTail& fileTail,
                                   const PixelsFooterCache& footerCache) {
    physicalReader = reader;
    footer = fileTail.footer();
    postScript = fileTail.postscript();
    pixelsFooterCache = footerCache;
}


/**
 * Prepare for the next row batch. This method is independent from readBatch().
 *
 * @param batchSize the willing batch size
 * @return the real batch size
 */
std::shared_ptr<PixelsRecordReader> PixelsReaderImpl::read(PixelsReaderOption option) {
    // TODO: add a function parameter, and the code before creating PixelsRecordReaderImpl
	std::shared_ptr<PixelsRecordReader> recordReader =
	    std::make_shared<PixelsRecordReaderImpl>(
            physicalReader, postScript,
            footer, option, pixelsFooterCache);
    recordReaders.emplace_back(recordReader);
    return recordReader;
}

