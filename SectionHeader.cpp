#include "SectionHeader.h"
#include <cstring>

SectionHeader::SectionHeader(eElfSectionType_t arg_Type, Word arg_NameIndex)
        : name{arg_NameIndex}, type{arg_Type}, flags{eElfSectionFlags_t::SHF_UNDEFINED}, addr{0U},
          offset{0U}, size{0U}, link{0U}, info{0U}, addrAlign{0U}, entsize{0U} {
}

SectionHeader::SectionHeader() : SectionHeader(eElfSectionType_t::SHT_NULL, 0U) {
}

uint32_t SectionHeader::CopyToBufferAt(uint8_t *arg_pBuffer) const {
    uint32_t tmp_Index = 0U;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&name), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&type), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&flags), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&addr), st_cOffsetAddr);
    tmp_Index += st_cOffsetAddr;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&offset), st_cOffsetOffs);
    tmp_Index += st_cOffsetOffs;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&size), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&link), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&info), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&addrAlign), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&entsize), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    return tmp_Index;
}

void SectionHeader::SetOffset(Offs arg_Offset) {
    offset = arg_Offset;
}

void SectionHeader::SetSize(Word arg_Size) {
    size = arg_Size;
}

void SectionHeader::AddFlag(eElfSectionFlags_t arg_Flag) {
    flags = static_cast<eElfSectionFlags_t>(static_cast<Word>(flags) | static_cast<Word>(arg_Flag));
}

void SectionHeader::SetAddr(Addr arg_Addr) {
    addr = arg_Addr;
}

void SectionHeader::SetAddrAlign(Word arg_AddrAlign) {
    addrAlign = arg_AddrAlign;
}
