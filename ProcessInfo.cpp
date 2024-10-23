#include <cstring>
#include "ProcessInfo.h"

ProcessInfo::ProcessInfo()
        : state{0U}, sname{0U}, zomb{0U}, nice{0U}, flag{0U}, uid{0U}, gid{0U}, pid{0U}, ppid{0U}, pgrp{0U}, sid{0U},
          fname{0U}, psargs{0U} {
}

uint32_t ProcessInfo::CopyAsNoteSectionToBuffer(uint8_t *arg_pBuffer, NoteSection &arg_NoteSection) {
    uint32_t tmp_Index = arg_NoteSection.CopyToBuffer(arg_pBuffer);

    (void) std::memcpy(&arg_pBuffer[tmp_Index], &state, 1U);
    tmp_Index += 1U;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], &sname, 1U);
    tmp_Index += 1U;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], &zomb, 1U);
    tmp_Index += 1U;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], &nice, 1U);
    tmp_Index += 1U;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * ) & flag, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * ) & uid, st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * ) & gid, st_cOffsetHalf);
    tmp_Index += st_cOffsetHalf;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * ) & pid, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * ) & ppid, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * ) & pgrp, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t * ) & sid, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    (void) std::memcpy(&arg_pBuffer[tmp_Index], &fname[0], 16U);
    tmp_Index += 16U;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], &psargs[0], 80U);
    tmp_Index += 80U;

    return tmp_Index;
}

void ProcessInfo::CopyFname(const std::string &arg_String) {
    (void) std::memcpy(&fname[0], reinterpret_cast<const uint8_t *>(arg_String.c_str()), arg_String.size() + 1U);
}

void ProcessInfo::CopyPsargs(const std::string &arg_String) {
    (void) std::memcpy(&psargs[0], reinterpret_cast<const uint8_t *>(arg_String.c_str()), arg_String.size() + 1U);
}

Word ProcessInfo::GetTypeForNoteSection() {
    return 3U;
}
