//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_PHYSICALREADERUTIL_H
#define PIXELS_PHYSICALREADERUTIL_H

#include "io/PhysicalLocalReader.h"
#include "Storage.h"
class PhysicalReaderUtil {
public:
    static PhysicalReader * newPhysicalReader(Storage * storage, std::string path) {
        if(storage == nullptr) {
            throw std::runtime_error("storage should not be nullptr");
        }
        if(path.size() == 0) {
            throw std::runtime_error("path should not be empty");
        }
        PhysicalReader * reader;
        switch (storage->getScheme()) {
            case Storage::hdfs:
                throw std::runtime_error("hdfs not support");
                break;
            case Storage::file:
                reader = new PhysicalLocalReader(storage, path);
                break;
            case Storage::s3:
                throw std::runtime_error("hdfs not support");
                break;
            case Storage::minio:
                throw std::runtime_error("hdfs not support");
                break;
            case Storage::redis:
                throw std::runtime_error("hdfs not support");
                break;
            case Storage::gcs:
                throw std::runtime_error("hdfs not support");
                break;
            case Storage::mock:
                throw std::runtime_error("hdfs not support");
                break;
            default:
                throw std::runtime_error("hdfs not support");
        }
        return reader;
    }

    static PhysicalReader newPhysicalReader(Storage::Scheme scheme, std::string path) {
        if(path.size() == 0) {
            throw std::runtime_error("path should not be empty");
        }
        // TODO: StorageFactory
    }
};
#endif //PIXELS_PHYSICALREADERUTIL_H
