#include <cstring>
#include "ProcessStatus.h"
#include "NoteSection.h"

SigInfo::SigInfo() : signo{0U}, code{0U}, errnumber{0U} {
}

TimeVal::TimeVal() : sec{0U}, usec{0U} {
}

ProcessStatus::ProcessStatus()
        : info{}, cursig{0U}, sigpend{0U}, sighold{0U}, pid{0U}, ppid{0U}, pgrp{0U}, sid{0U}, utime{}, stime{},
          cutime{}, cstime{}, reg{0U}, fpvalid{0U} {
}

uint32_t ProcessStatus::CopyAsNoteSectionToBuffer(uint8_t *arg_pBuffer, NoteSection &arg_NoteSection) {
    uint32_t tmp_Index = arg_NoteSection.CopyToBuffer(arg_pBuffer);

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &info.signo, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &info.code, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &info.errnumber, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &cursig, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &sigpend, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &sighold, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &pid, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &ppid, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &pgrp, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &sid, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &utime.sec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &utime.usec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &stime.sec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &stime.usec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &cutime.sec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &cutime.usec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &cstime.sec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &cstime.usec, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    for (Word &tmp_Reg: reg) {
        (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &tmp_Reg, st_cOffsetWord);
        tmp_Index += st_cOffsetWord;
    }

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &fpvalid, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    return tmp_Index;
}

void ProcessStatus::ReadMemory(const uint32_t *arg_pRegisters) {
    for (uint8_t tmp_I = 0U; tmp_I < 18; ++tmp_I) {
        reg[tmp_I] = arg_pRegisters[tmp_I];
    }
}

Word ProcessStatus::GetTypeForNoteSection() {
    return 1U;
}
