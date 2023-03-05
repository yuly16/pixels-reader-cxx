//
// Created by liyu on 2/28/23.
//

#include "physical/storage/LocalFS.h"
#include "physical/natives/DirectRandomAccessFile.h"


std::string LocalFS::SchemePrefix = "file://";

LocalFS::LocalFS() {

};

Storage::Scheme LocalFS::getScheme() {
    return file;
}

std::string LocalFS::ensureSchemePrefix(std::string path) {
    if(path.rfind(SchemePrefix, 0) != std::string::npos) {
        return path;
    }
    if(path.find("://") != std::string::npos) {
        throw std::invalid_argument("Path '" + path +
                             "' already has a different scheme prefix than '" + SchemePrefix + "'.");
    }
    return SchemePrefix + path;
}

PixelsRandomAccessFile * LocalFS::openRaf(const std::string& path) {
    if(true) {
        // TODO: change this class to mmap class in the future.
        return new DirectRandomAccessFile(path);
    } else {
        return new DirectRandomAccessFile(path);
    }
}