#include "ElfHeader.h"
#include <cstring>

ElfHeader::ElfHeader()
        : ident{}, type{eDElfHeaderType_t::ET_CORE}, machine{eElfHeaderMachine_t::EM_ARM},
          version{eElfHeaderVersion_t::EV_CURRENT}, entry{0U}, phoff{0U}, shoff{0U}, flags{0U},
          ehsize{st_cElfHeaderSize},
          phentsize{st_cProgramHeaderSize}, phnum{0U}, shentsize{st_cSectionHeaderSize}, shnum{0U}, shstrndx{0U} {
}

uint32_t ElfHeader::CopyToBufferAt(uint8_t *arg_pBuffer) const {
    uint32_t tmp_Index = 0U;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], &ident.data[0], 16U * sizeof(uint8_t));
    tmp_Index += 16U;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&type), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&machine), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&version), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&entry), st_cOffsetAddr);
    tmp_Index += st_cOffsetAddr;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&phoff), st_cOffsetOffs);
    tmp_Index += st_cOffsetOffs;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&shoff), st_cOffsetOffs);
    tmp_Index += st_cOffsetOffs;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&flags), st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&ehsize), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&phentsize), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&phnum), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&shentsize), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&shnum), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) (&shstrndx), st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;

    return tmp_Index;
}

const Ident &ElfHeader::GetIdent() const {
    return ident;
}

eDElfHeaderType_t ElfHeader::GetType() const {
    return type;
}

eElfHeaderMachine_t ElfHeader::GetMachine() const {
    return machine;
}

eElfHeaderVersion_t ElfHeader::GetVersion() const {
    return version;
}

Addr ElfHeader::GetEntry() const {
    return entry;
}

Offs ElfHeader::GetPhoff() const {
    return phoff;
}

Offs ElfHeader::GetShoff() const {
    return shoff;
}

Word ElfHeader::GetFlags() const {
    return flags;
}

Half ElfHeader::GetEhsize() const {
    return ehsize;
}

Half ElfHeader::GetPhentsize() const {
    return phentsize;
}

Half ElfHeader::GetPhnum() const {
    return phnum;
}

Half ElfHeader::GetShentsize() const {
    return shentsize;
}

Half ElfHeader::GetShnum() const {
    return shnum;
}

Half ElfHeader::GetShstrndx() const {
    return shstrndx;
}

void ElfHeader::SetPhoff(Offs arg_Phoff) {
    phoff = arg_Phoff;
}

void ElfHeader::SetShoff(Offs arg_Shoff) {
    shoff = arg_Shoff;
}

void ElfHeader::IncShnum() {
    shnum++;
}

void ElfHeader::IncPhnum() {
    phnum++;
}

void ElfHeader::SetShstrndx(Half arg_shstrndx) {
    shstrndx = arg_shstrndx;
}
