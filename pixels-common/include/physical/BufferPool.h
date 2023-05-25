//
// Created by liyu on 5/25/23.
//

#ifndef DUCKDB_BUFFERPOOL_H
#define DUCKDB_BUFFERPOOL_H

#include <iostream>
#include <vector>
#include "physical/natives/ByteBuffer.h"
#include <memory>
#include "physical/natives/DirectIoLib.h"
#include "exception/InvalidArgumentException.h"
#include <map>
// This class is global class. The variable is shared by each thread
class BufferPool {
public:
	static BufferPool & Instance();
	static void Initialize(uint32_t rgCount, std::vector<uint32_t> colIds, std::vector<uint64_t> bytes);
	static std::shared_ptr<ByteBuffer> GetBuffer(uint32_t rgId, uint32_t colId);
	static void Reset();
private:
	BufferPool() = default;
	static thread_local int colCount;
	static thread_local int rgCount;
	static thread_local std::vector<std::map<uint32_t, uint64_t>> nrBytes;
	static thread_local bool isInitialized;
	static thread_local std::vector<std::map<uint32_t, std::shared_ptr<ByteBuffer>>> buffers;
	static std::shared_ptr<DirectIoLib> directIoLib;
};
#endif // DUCKDB_BUFFERPOOL_H
