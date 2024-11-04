#ifndef MEMORYINFORMATION_H
#define MEMORYINFORMATION_H

#include <cstdint>

struct MemoryInformation {
    uint32_t StackEndAddress;
    uint32_t StackSize;
    uint32_t DataEndAddress;
    uint32_t DataSize;
};


#endif //MEMORYINFORMATION_H
