//
// Created by liyu on 3/1/23.
//
#include "physical/Storage.h"

std::map<std::string, Storage::Scheme> Storage::schemeMap = {
    {"hdfs", Storage::hdfs},
    {"file", Storage::file},
    {"s3", Storage::s3},
    {"minio", Storage::minio},
    {"redis", Storage::redis},
    {"gcs", Storage::gcs},
    {"mock", Storage::mock},
};

Storage::Scheme Storage::from(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return Storage::schemeMap[value];
}

Storage::Scheme Storage::fromPath(std::string schemedPath) {
    std::string scheme = schemedPath.substr(0, schemedPath.indexOf("://"));
    return Scheme.from(scheme);
}