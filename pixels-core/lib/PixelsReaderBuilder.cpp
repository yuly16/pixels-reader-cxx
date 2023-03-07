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

    return new PixelsReaderImpl(fsReader);
}

