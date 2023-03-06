//
// Created by liyu on 3/6/23.
//

#include "PixelsReaderBuilder.h"

PixelsReaderBuilder::PixelsReaderBuilder() {
    buildStorage = nullptr;
}

PixelsReaderBuilder *PixelsReaderBuilder::setStorage(Storage *storage) {
    buildStorage = storage;
    return this;
}

PixelsReader *PixelsReaderBuilder::build() {
    // TODO: this is too simple. Please finish all variables in PixelsReaderBuilder
    PhysicalReader * fsReader =
    return new PixelsReaderImpl(*buildStorage);
}
