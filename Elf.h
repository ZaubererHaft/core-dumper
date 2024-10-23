/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details
 *
 * \author   l.kratzl
 */

#ifndef HCTRLXCDELF_H_
#define HCTRLXCDELF_H_

#include <utility>

#include "ElfHeader.h"
#include "ProgramHeader.h"
#include "SectionHeader.h"

class Elf {
public:
    explicit Elf(const ElfHeader &arg_Header);

    uint8_t AddSectionHeader(SectionHeader &arg_Header);

    uint8_t AddProgramHeader(ProgramHeader &arg_Header);

    uint8_t AddGenericPayload(const uint8_t *arg_pPayload, uint32_t arg_PayloadLength);

    void LinkSectionHeaderWithPayload(uint8_t arg_SectionIndex, uint8_t arg_PayloadIndex);

    void LinkProgramHeaderWithPayload(uint8_t arg_SectionIndex, uint8_t arg_PayloadIndex);

    uint32_t WriteToBuffer(uint8_t *arg_pBuffer);

    uint32_t CalculateSize();

private:
    uint32_t calcOffsetUntilPayloadIndex(uint8_t arg_Index);

    static constexpr uint8_t st_cMaxSectionHeaders = 6U;
    static constexpr uint8_t st_cMaxSectionPayloads = 6U;
    static constexpr uint8_t st_cMaxProgramHeaders = 3U;
    static constexpr uint8_t st_cMaxProgramPayloads = 3U;
    static constexpr uint8_t st_cMaxPayloads = 6U;

    ElfHeader header;
    SectionHeader sectionHeaders[st_cMaxSectionHeaders];
    ProgramHeader programHeaders[st_cMaxProgramHeaders];
    int8_t sectionPayloads[st_cMaxSectionPayloads];
    int8_t programPayloads[st_cMaxProgramPayloads];
    std::pair<const uint8_t *, uint32_t> payloads[st_cMaxPayloads];
    uint8_t sectionHeaderCount;
    uint8_t programHeaderCount;
    uint8_t payloadCount;
};

#endif // HCTRLXCDELF_H_

/**
 * \}
 * \}
 */