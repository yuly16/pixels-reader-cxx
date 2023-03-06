//
// Created by liyu on 3/6/23.
//

#include "physical/StorageFactory.h"

StorageFactory * StorageFactory::instance = nullptr;

StorageFactory::StorageFactory() {
    //TODO: read enabled.storage.schemes from pixels.properties
    enabledSchemes.insert(Storage::file);
}

StorageFactory * StorageFactory::getInstance() {
    if(instance == nullptr) {
        instance = new StorageFactory();
        //TODO: close all here.
    }
    return instance;
}
