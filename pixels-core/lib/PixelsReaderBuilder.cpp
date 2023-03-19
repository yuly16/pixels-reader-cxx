//
// Created by liyu on 3/6/23.
//

#include "PixelsReaderBuilder.h"

PixelsReaderBuilder::PixelsReaderBuilder() {
    builderStorage = nullptr;
    builderPath = "";
}

PixelsReaderBuilder * PixelsReaderBuilder::setStorage(Storage *storage) {
    builderStorage = storage;
    return this;
}

PixelsReaderBuilder * PixelsReaderBuilder::setPath(const std::string &path) {
    builderPath = path;
    return this;
}


PixelsReaderBuilder *PixelsReaderBuilder::setPixelsFooterCache(const PixelsFooterCache &pixelsFooterCache) {
    builderPixelsFooterCache = pixelsFooterCache;
    return this;
}

PixelsReader * PixelsReaderBuilder::build() {
    if(builderStorage == nullptr || builderPath.empty()) {
        throw std::runtime_error("Missing argument to build PixelsReader");
    }
    // get PhysicalReader
    PhysicalReader * fsReader = PhysicalReaderUtil::newPhysicalReader(
            builderStorage, builderPath);
    // try to get file tail from cache
    std::string fileName = fsReader->getName();
    pixels::proto::FileTail fileTail;
    if(builderPixelsFooterCache.containsFileTail(fileName)) {
        fileTail = builderPixelsFooterCache.getFileTail(fileName);
    } else {
        if(fsReader == nullptr) {
            throw PixelsReaderException(
                    "Failed to create PixelsReader due to error of creating PhysicalReader");
        }
        // get FileTail
        long fileLen = fsReader->getFileLength();
        fsReader->seek(fileLen - (long)sizeof(long));
        long fileTailOffset = fsReader->readLong();
        int fileTailLength = (int) (fileLen - fileTailOffset - sizeof(long));
        fsReader->seek(fileTailOffset);
        std::shared_ptr<ByteBuffer> fileTailBuffer = fsReader->readFully(fileTailLength);
        if(!fileTail.ParseFromArray(fileTailBuffer->getPointer(),
                                    fileTailLength)) {
            throw InvalidArgumentException("paring FileTail error!");
        }
        builderPixelsFooterCache.putFileTail(fileName, fileTail);
    }

    // check file MAGIC and file version
    pixels::proto::PostScript postScript = fileTail.postscript();
    uint32_t fileVersion = postScript.version();
    const std::string& fileMagic = postScript.magic();
    if(PixelsVersion::currentVersion() != fileVersion) {
        throw PixelsFileVersionInvalidException(fileVersion);
    }
    if(fileMagic != Constants::MAGIC) {
        throw PixelsFileMagicInvalidException(fileMagic);
    }

    // TODO: the remaining things, such as builderSchema, coreCOnfig, metric
    return new PixelsReaderImpl(fsReader, fileTail,
                                builderPixelsFooterCache);
}


