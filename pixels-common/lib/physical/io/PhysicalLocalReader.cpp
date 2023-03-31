//
// Created by liyu on 2/27/23.
//
#include "physical/storage/LocalFS.h"
#include "physical/io/PhysicalLocalReader.h"

#include <utility>

PhysicalLocalReader::PhysicalLocalReader(std::shared_ptr<Storage> storage, std::string path_) {
    // TODO: should support async
    if(std::dynamic_pointer_cast<LocalFS>(storage).get() != nullptr) {
        local = std::dynamic_pointer_cast<LocalFS>(storage);
    } else {
        throw std::runtime_error("Storage is not LocalFS.");
    }
    if(path_.rfind("file://", 0) != std::string::npos) {
        // remove the scheme.
        path_.erase(0, 7);
    }
    path = std::move(path_);
    raf = local->openRaf(path);
    // TODO: get fileid.
    numRequests = 1;
}

std::shared_ptr<ByteBuffer> PhysicalLocalReader::readFully(int length) {
    numRequests++;
    return raf->readFully(length);
}

void PhysicalLocalReader::close() {
    numRequests++;
    raf->close();
}

long PhysicalLocalReader::getFileLength() {
    numRequests++;
    return raf->length();
}

void PhysicalLocalReader::seek(long desired) {
    numRequests++;
    raf->seek(desired);
}

long PhysicalLocalReader::readLong() {
    return raf->readLong();
}

char PhysicalLocalReader::readChar() {
    return raf->readChar();
}

int PhysicalLocalReader::readInt() {
    return raf->readInt();
}

std::string PhysicalLocalReader::getName() {
    if(path.empty()) {
        return "";
    }
    return path.substr(path.find_last_of('/') + 1);
}
