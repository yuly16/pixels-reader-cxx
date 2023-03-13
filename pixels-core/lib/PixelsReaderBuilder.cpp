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


PixelsReader * PixelsReaderBuilder::build() {
    if(builderStorage == nullptr || builderPath.empty()) {
        throw std::runtime_error("Missing argument to build PixelsReader");
    }
    // get PhysicalReader
    PhysicalReader * fsReader = PhysicalReaderUtil::newPhysicalReader(
            builderStorage, builderPath);
    // TODO: PixelsProto and metric files
    // TODO: we need concurrenthashmap for builderPixelsFooterCache
    pixels::proto::FileTail fileTail;
    if(fsReader == nullptr) {
        throw std::runtime_error("Failed to create PixelsReader due to error of creating PhysicalReader");
    }
    // get FileTail
    long fileLen = fsReader->getFileLength();
    fsReader->seek(fileLen - (long)sizeof(long));
    long fileTailOffset = fsReader->readLong();
    int fileTailLength = (int) (fileLen - fileTailOffset - sizeof(long));
    fsReader->seek(fileTailOffset);
    ByteBuffer * fileTailBuffer = fsReader->readFully(fileTailLength);
    if(!fileTail.ParseFromArray(fileTailBuffer->getPointer(),
                               fileTailLength)) {
        throw std::runtime_error("paring FileTail error!");
    }

    // check file MAGIC and file version
    pixels::proto::PostScript postScript = fileTail.postscript();
    uint32_t fileVersion = postScript.version();
    std::string fileMagic = postScript.magic();
    if(PixelsVersion::currentVersion() != fileVersion) {

    }
    return new PixelsReaderImpl(fsReader);
}

