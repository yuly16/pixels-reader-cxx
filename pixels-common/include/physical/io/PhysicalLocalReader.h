//
// Created by liyu on 2/27/23.
//

#ifndef PIXELS_READER_PHYSICALLOCALREADER_H
#define PIXELS_READER_PHYSICALLOCALREADER_H

#include "physical/PhysicalReader.h"
#include "physical/storage/LocalFS.h"

class PhysicalLocalReader: public PhysicalReader {
public:

private:
    LocalFS local;
    std::string path;
    long id;
};

#endif //PIXELS_READER_PHYSICALLOCALREADER_H
