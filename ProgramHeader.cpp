#include "ProgramHeader.h"
#include <cstring>

ProgramHeader::ProgramHeader(eElfProgramType_t arg_Type, eElfProgramPermissions_t arg_Flag)
        : type{arg_Type}, offset{0U}, vaddr{0U}, paddr{0U}, filesize{0U}, memsize{0U}, flags{arg_Flag}, align{0U} {
}

ProgramHeader::ProgramHeader() : ProgramHeader(eElfProgramType_t::PT_NULL, eElfProgramPermissions_t::NONE) {
}

uint32_t ProgramHeader::CopyToBufferAt(uint8_t *arg_pBuffer) const {
    uint32_t tmp_Index = 0U;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&type), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&offset), st_cOffsetOffs);
    tmp_Index += st_cOffsetOffs;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&vaddr), st_cOffsetAddr);
    tmp_Index += st_cOffsetAddr;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&paddr), st_cOffsetAddr);
    tmp_Index += st_cOffsetAddr;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&filesize), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&memsize), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&flags), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * )(&align), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    return tmp_Index;
}

void ProgramHeader::SetOffset(Offs arg_Offset) {
    offset = arg_Offset;
}

void ProgramHeader::SetFilesize(Word arg_Filesize) {
    filesize = arg_Filesize;
}

void ProgramHeader::SetMemsize(Word arg_Memsize) {
    memsize = arg_Memsize;
}

void ProgramHeader::SetVaddr(Addr arg_Vaddr) {
    vaddr = arg_Vaddr;
}

void ProgramHeader::SetPaddr(Addr arg_Paddr) {
    paddr = arg_Paddr;
}

void ProgramHeader::SetAlign(Word arg_Align) {
    align = arg_Align;
}
