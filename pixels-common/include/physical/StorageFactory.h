//
// Created by liyu on 3/6/23.
//

#ifndef PIXELS_STORAGEFACTORY_H
#define PIXELS_STORAGEFACTORY_H
#include <bits/stdc++.h>
#include "physical/Storage.h"

class StorageFactory {
public:
    static StorageFactory * getInstance();
private:
    //TODO: logger
    StorageFactory();
    std::unordered_map<Storage::Scheme, Storage *> storageImpls;
    std::set<Storage::Scheme> enabledSchemes;
    static StorageFactory * instance;

};
#endif //PIXELS_STORAGEFACTORY_H
