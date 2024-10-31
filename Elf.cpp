#include "Elf.h"

#include <cstring>
#include "crash.h"

Elf::Elf(const ElfHeader &arg_Header)
        : header(arg_Header), sectionHeaders{}, programHeaders{}, sectionPayloads{-1}, programPayloads{-1},
          payloads{{nullptr, 0}},
          sectionHeaderCount{0U}, programHeaderCount{0U}, payloadCount{0U} {
    header.SetShoff(st_cElfHeaderSize);

    (void) std::memset(&sectionPayloads[0], -1, st_cMaxSectionPayloads);
    (void) std::memset(&programPayloads[0], -1, st_cMaxProgramPayloads);
}

uint8_t Elf::AddSectionHeader(const SectionHeader &arg_Header) {
    if (sectionHeaderCount >= st_cMaxSectionHeaders) {
        Crash();
    }

    header.IncShnum();
    uint8_t tmp_Index = sectionHeaderCount;

    sectionHeaders[tmp_Index] = arg_Header;
    sectionHeaderCount++;
    header.SetPhoff(header.GetEhsize() + header.GetShnum() * header.GetShentsize());
    return tmp_Index;
}

uint8_t Elf::AddProgramHeader(const ProgramHeader &arg_Header) {
    if (programHeaderCount >= st_cMaxProgramHeaders) {
        Crash();
    }

    header.IncPhnum();
    uint8_t tmp_Index = programHeaderCount;

    programHeaders[tmp_Index] = arg_Header;
    programHeaderCount++;
    return tmp_Index;
}

uint8_t Elf::AddGenericPayload(const uint8_t *arg_pPayload, uint32_t arg_PayloadLength) {
    if (payloadCount >= st_cMaxPayloads) {
        Crash();
    }

    uint8_t tmp_Index = payloadCount;
    payloads[tmp_Index] = {arg_pPayload, arg_PayloadLength};
    payloadCount++;
    return tmp_Index;
}

void Elf::LinkSectionHeaderWithPayload(uint8_t arg_SectionIndex, uint8_t arg_PayloadIndex) {
    if (arg_SectionIndex >= st_cMaxSectionPayloads) {
        Crash();
    }
    if (sectionPayloads[arg_SectionIndex] > 0) {
        Crash(); //already in use
    }

    sectionPayloads[arg_SectionIndex] = static_cast<int8_t>(arg_PayloadIndex);
    sectionHeaders[arg_SectionIndex].SetSize(payloads[arg_PayloadIndex].second);
}

void Elf::LinkProgramHeaderWithPayload(uint8_t arg_SectionIndex, uint8_t arg_PayloadIndex) {
    if (arg_SectionIndex >= st_cMaxProgramPayloads) {
        Crash();
    }
    if (programPayloads[arg_SectionIndex] > 0) {
        Crash(); //already in use
    }

    programPayloads[arg_SectionIndex] = static_cast<int8_t>(arg_PayloadIndex);
    programHeaders[arg_SectionIndex].SetMemsize(payloads[arg_PayloadIndex].second);
    programHeaders[arg_SectionIndex].SetFilesize(payloads[arg_PayloadIndex].second);
}

uint32_t Elf::WriteToBuffer(uint8_t *arg_pBuffer) {
    uint32_t tmp_Index = 0U;

    // write header
    tmp_Index += header.CopyToBufferAt(&arg_pBuffer[0]);

    // write section headers
    for (uint8_t tmp_I = 0U; tmp_I < sectionHeaderCount; ++tmp_I) {
        SectionHeader &tmp_Section = sectionHeaders[tmp_I];
        int8_t tmp_PayloadIndex = sectionPayloads[tmp_I];
        if (tmp_PayloadIndex >= 0) {
            tmp_Section.SetOffset(calcOffsetUntilPayloadIndex(tmp_PayloadIndex));
        }

        tmp_Index += tmp_Section.CopyToBufferAt(&arg_pBuffer[tmp_Index]);
    }

    // write program header
    for (uint8_t tmp_I = 0U; tmp_I < programHeaderCount; ++tmp_I) {
        ProgramHeader &tmp_Program = programHeaders[tmp_I];
        int8_t tmp_PayloadIndex = programPayloads[tmp_I];
        if (tmp_PayloadIndex >= 0) {
            tmp_Program.SetOffset(calcOffsetUntilPayloadIndex(tmp_PayloadIndex));
        }
        tmp_Index += tmp_Program.CopyToBufferAt(&arg_pBuffer[tmp_Index]);
    }

    // write payloads
    for (uint8_t tmp_I = 0U; tmp_I < payloadCount; ++tmp_I) {
        auto [tmp_buffer, tmp_length] = payloads[tmp_I];
        if (tmp_buffer != nullptr) {
            // manual copy to allow dest = src ptr
            for (uint32_t tmp_J = 0U; tmp_J < tmp_length; ++tmp_J) {
                arg_pBuffer[tmp_Index + tmp_J] = tmp_buffer[tmp_J];
            }
            tmp_Index += tmp_length;
        }
    }

    return tmp_Index;
}

uint32_t Elf::calcOffsetUntilPayloadIndex(uint8_t arg_Index) {
    Word tmp_Offset = header.GetShentsize() * header.GetShnum() + header.GetPhnum() * header.GetPhentsize() + header.
            GetEhsize();

    for (uint8_t tmp_I = 0U; tmp_I < arg_Index; ++tmp_I) {
        auto &tmp_Payload = payloads[tmp_I];
        if (tmp_Payload.first != nullptr) {
            tmp_Offset += tmp_Payload.second;
        }
    }

    return tmp_Offset;
}

uint32_t Elf::CalculateSize() {
    return calcOffsetUntilPayloadIndex(payloadCount);
}
