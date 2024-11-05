#include "CoreWriter.h"
#include "Elf.h"
#include "NoteSection.h"
#include "ProcessInfo.h"
#include "ProcessStatus.h"

extern "C" void
CreateDump(uint32_t *arg_pBufferForRegisters, uint8_t *arg_pBufferForStack, uint8_t *arg_pBufferForData,
           uint32_t *arg_pAddresses);

CoreWriter::CoreWriter(const MemoryInformation &arg_MemInfo) : memInfo{arg_MemInfo}, elf{createElf()}, addresses{
        arg_MemInfo.StackEndAddress - arg_MemInfo.StackSize, arg_MemInfo.StackEndAddress,
        arg_MemInfo.DataEndAddress - arg_MemInfo.DataSize, arg_MemInfo.DataEndAddress} {
}

Elf CoreWriter::createElf() {
    ElfHeader tmp_Header{};
    tmp_Header.SetShstrndx(st_cIndexStringSectionHeader);
    Elf tmp_Elf{tmp_Header};
    return tmp_Elf;
}

uint32_t CoreWriter::GetMaxRequiredNumberOfBytes() const {
    return getNumberOfBytesUntilPayload() + memInfo.StackSize + memInfo.DataSize + st_cStringTableSize +
           st_cNoteSectionSize;
}

constexpr uint32_t CoreWriter::getNumberOfBytesUntilPayload() {
    return ElfHeader::Size() + st_cCountProgramHeaders * ProgramHeader::Size() +
           st_cCountSectionHeaders * SectionHeader::Size();
}

uint32_t CoreWriter::Write(uint8_t *arg_pBuffer, uint32_t arg_BuffLen) {
    uint32_t tmp_Written = 0U;

    elf = createElf();

    uint32_t tmp_OffsetUntilPayload = getNumberOfBytesUntilPayload();
    uint32_t tmp_StackSize = addresses[1] - addresses[0];
    uint32_t tmp_DataSize = addresses[3] - addresses[2];

    //make sure that the buffer is sufficient at least for the dump and headers
    if (tmp_OffsetUntilPayload + tmp_DataSize + tmp_StackSize < arg_BuffLen) {

        //write dump into the provided buffer and save the registers for later
        uint32_t tmp_Registers[18U] = {0U};
        CreateDump(&tmp_Registers[0], &arg_pBuffer[tmp_OffsetUntilPayload],
                   &arg_pBuffer[tmp_OffsetUntilPayload + memInfo.StackSize], addresses);

        // add the stack and data content as payload into the elf
        uint8_t tmp_StackProgramHeaderPayloadIndex = elf.AddGenericPayload(&arg_pBuffer[tmp_OffsetUntilPayload],
                                                                           memInfo.StackSize);
        uint8_t tmp_DataProgramHeaderPayloadIndex = elf.AddGenericPayload(
                &arg_pBuffer[tmp_OffsetUntilPayload + memInfo.StackSize], memInfo.DataSize);

        //create the section header
        uint8_t tmp_SectionHeaderIndices[st_cCountSectionHeaders];
        createSectionHeaders(tmp_SectionHeaderIndices);

        //create the program header
        uint8_t tmp_ProgramHeaderIndices[st_cCountProgramHeaders];
        createProgramHeaders(tmp_ProgramHeaderIndices);

        //create the process status and process info as payload into a second buffer and add them as note section
        uint8_t tmp_NoteSectionBuffer[st_cNoteSectionSize] = {0U};
        uint32_t tmp_NoteBuffLen = createNoteSectionsPayload(&tmp_NoteSectionBuffer[0], &tmp_Registers[0]);
        uint8_t tmp_NotePayloadIndex = elf.AddGenericPayload(&tmp_NoteSectionBuffer[0], tmp_NoteBuffLen);

        // add the string table as payload
        uint8_t tmp_StringTablePayloadIndex = elf.AddGenericPayload(
                reinterpret_cast<const uint8_t *>(&st_cStringTable[0]),
                st_cStringTableSize);

        // connect the payloads with the respective section or program headers
        elf.LinkProgramHeaderWithPayload(tmp_ProgramHeaderIndices[st_cIndexStackProgramHeader],
                                         tmp_StackProgramHeaderPayloadIndex);
        elf.LinkSectionHeaderWithPayload(tmp_SectionHeaderIndices[st_cIndexStackSectionHeader],
                                         tmp_StackProgramHeaderPayloadIndex);

        elf.LinkProgramHeaderWithPayload(tmp_ProgramHeaderIndices[st_cIndexDataProgramHeader],
                                         tmp_DataProgramHeaderPayloadIndex);
        elf.LinkSectionHeaderWithPayload(tmp_SectionHeaderIndices[st_cIndexDataSectionHeader],
                                         tmp_DataProgramHeaderPayloadIndex);

        elf.LinkSectionHeaderWithPayload(tmp_SectionHeaderIndices[st_cIndexStringSectionHeader],
                                         tmp_StringTablePayloadIndex);

        elf.LinkProgramHeaderWithPayload(tmp_ProgramHeaderIndices[st_cIndexNoteProgramHeader], tmp_NotePayloadIndex);
        elf.LinkSectionHeaderWithPayload(tmp_SectionHeaderIndices[st_cIndexNoteSectionHeader], tmp_NotePayloadIndex);

        // Final check: is the overall size of the buffer sufficient? If yes, add the rest as well
        uint32_t tmp_TargetLen = elf.CalculateSize();
        if (tmp_TargetLen <= arg_BuffLen) {
            tmp_Written = elf.WriteToBuffer(arg_pBuffer);
        }
    }

    return tmp_Written;
}

void CoreWriter::createSectionHeaders(uint8_t arg_pSectionHeaderIndices[st_cCountSectionHeaders]) {
    SectionHeader tmp_NullSectionHeader{};
    arg_pSectionHeaderIndices[st_cIndexNullSectionHeader] = elf.AddSectionHeader(tmp_NullSectionHeader);

    SectionHeader tmp_StringSectionHeader{eElfSectionType_t::SHT_STRTAB,
                                          st_cIndexStringTableNameInStringTable};
    arg_pSectionHeaderIndices[st_cIndexStringSectionHeader] = elf.AddSectionHeader(tmp_StringSectionHeader);

    SectionHeader tmp_NoteSectionHeader{eElfSectionType_t::SHT_NOTE,
                                        st_cIndexNoteSectionNameInStringTable};
    tmp_NoteSectionHeader.AddFlag(eElfSectionFlags_t::SHF_ALLOC);
    arg_pSectionHeaderIndices[st_cIndexNoteSectionHeader] = elf.AddSectionHeader(tmp_NoteSectionHeader);

    SectionHeader tmp_StackSectionHeader{eElfSectionType_t::SHT_PROGBITS,
                                         st_cIndexStackSectionNameInStringTable};
    tmp_StackSectionHeader.AddFlag(eElfSectionFlags_t::SHF_ALLOC);
    tmp_StackSectionHeader.AddFlag(eElfSectionFlags_t::SHF_WRITE);
    tmp_StackSectionHeader.SetAddr(memInfo.StackEndAddress - memInfo.StackSize);
    tmp_StackSectionHeader.SetSize(memInfo.StackSize);
    tmp_StackSectionHeader.SetAddrAlign(4U);
    arg_pSectionHeaderIndices[st_cIndexStackSectionHeader] = elf.AddSectionHeader(tmp_StackSectionHeader);

    SectionHeader tmp_DataSectionHeader{eElfSectionType_t::SHT_PROGBITS,
                                        st_cIndexDataSectionNameInStringTable};
    tmp_DataSectionHeader.AddFlag(eElfSectionFlags_t::SHF_ALLOC);
    tmp_DataSectionHeader.AddFlag(eElfSectionFlags_t::SHF_WRITE);
    tmp_DataSectionHeader.SetAddr(memInfo.DataEndAddress - memInfo.DataSize);
    tmp_DataSectionHeader.SetSize(memInfo.DataSize);
    tmp_DataSectionHeader.SetAddrAlign(4U);
    arg_pSectionHeaderIndices[st_cIndexDataSectionHeader] = elf.AddSectionHeader(tmp_DataSectionHeader);
}

void CoreWriter::createProgramHeaders(uint8_t arg_pSrogramHeaderIndices[st_cCountProgramHeaders]) {
    ProgramHeader tmp_NoteProgramHeader{eElfProgramType_t::PT_NOTE,
                                        eElfProgramPermissions_t::PF_R};
    arg_pSrogramHeaderIndices[st_cIndexNoteProgramHeader] = elf.AddProgramHeader(tmp_NoteProgramHeader);

    ProgramHeader tmp_StackProgramHeader{eElfProgramType_t::PT_LOAD,
                                         eElfProgramPermissions_t::PF_R_W};
    tmp_StackProgramHeader.SetPaddr(memInfo.StackEndAddress - memInfo.StackSize);
    tmp_StackProgramHeader.SetVaddr(memInfo.StackEndAddress - memInfo.StackSize);
    tmp_StackProgramHeader.SetAlign(4U);
    arg_pSrogramHeaderIndices[st_cIndexStackProgramHeader] = elf.AddProgramHeader(tmp_StackProgramHeader);

    ProgramHeader tmp_DataProgramHeader{eElfProgramType_t::PT_LOAD,
                                        eElfProgramPermissions_t::PF_R_W};
    tmp_DataProgramHeader.SetPaddr(memInfo.DataEndAddress - memInfo.DataSize);
    tmp_DataProgramHeader.SetVaddr(memInfo.DataEndAddress - memInfo.DataSize);
    tmp_DataProgramHeader.SetAlign(4U);
    arg_pSrogramHeaderIndices[st_cIndexDataProgramHeader] = elf.AddProgramHeader(tmp_DataProgramHeader);
}

uint32_t CoreWriter::createNoteSectionsPayload(uint8_t *arg_pBuffer, uint32_t *arg_pRegisters) {
    ProcessInfo tmp_Info{};
    tmp_Info.CopyFname("Appl");
    tmp_Info.CopyPsargs("Appl.elf");

    ProcessStatus tmp_Status{};
    tmp_Status.ReadMemory(arg_pRegisters);

    NoteSection tmp_PrpsinfoNote{"CORE", ProcessInfo::Size(),
                                 ProcessInfo::GetTypeForNoteSection()};
    NoteSection tmp_PrpstatusNote{"CORE", ProcessStatus::Size(),
                                  ProcessStatus::GetTypeForNoteSection()};

    uint32_t tmp_Index = tmp_Status.CopyAsNoteSectionToBuffer(arg_pBuffer, tmp_PrpstatusNote);
    return tmp_Index + tmp_Info.CopyAsNoteSectionToBuffer(&arg_pBuffer[tmp_Index], tmp_PrpsinfoNote);
}
