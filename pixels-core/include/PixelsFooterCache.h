//
// Created by liyu on 3/14/23.
//

#ifndef PIXELS_PIXELSFOOTERCACHE_H
#define PIXELS_PIXELSFOOTERCACHE_H

#include "tbb/concurrent_hash_map.h"
#include <iostream>
#include <string>
#include "pixels-common/pixels.pb.h"

using namespace tbb;
using namespace pixels::proto;
typedef concurrent_hash_map<std::string, FileTail> FileTailTable;
typedef concurrent_hash_map<std::string, RowGroupFooter> RGFooterTable;

class PixelsFooterCache {
public:
    PixelsFooterCache();
    void putFileTail(const std::string& id, FileTail fileTail);
    bool containsFileTail(const std::string& id);
    FileTail getFileTail(const std::string& id);
    void putRGFooter(const std::string& id, RowGroupFooter footer);
    bool containsRGFooter(const std::string& id);
    RowGroupFooter getRGFooter(const std::string& id);
private:
    FileTailTable fileTailCacheMap;
    RGFooterTable rowGroupFooterCacheMap;

};
#endif //PIXELS_PIXELSFOOTERCACHE_H
