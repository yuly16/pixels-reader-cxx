//
// Created by yuliangyong on 2023-03-02.
//
#include "physical/natives/DirectRandomAccessFile.h"
#include <cstdio>

DirectRandomAccessFile::DirectRandomAccessFile(const std::string& file) {
    // TODO: changing this flag to O_DIRECT when copying the code to linux. Windows doesn't support O_DIRECT
    fd = open(file.c_str(), O_RDONLY);
    FILE* fp = fopen(file.c_str(), "r");
    // checking if the file exist or not
    if (fp == NULL) {
        throw std::runtime_error("File not found");
    }
    fseek(fp, 0L, SEEK_END);
    // calculating the size of the file
    length = ftell(fp);
    // closing the file
    fclose(fp);
}

