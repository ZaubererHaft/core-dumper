#ifndef CORE_DUMP_MEMORYINFORMATION_H
#define CORE_DUMP_MEMORYINFORMATION_H

#include <cstdint>

struct MemoryInformation {
    uint32_t StackEndAddress;
    uint32_t StackSize;
    uint32_t DataEndAddress;
    uint32_t DataSize;
};


#endif //CORE_DUMP_MEMORYINFORMATION_H
