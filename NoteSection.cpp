#include <cstring>
#include "NoteSection.h"
#include "crash.h"

NoteSection::NoteSection() : namesz{0U}, descsz{0U}, type{0U}, name{} {
}

NoteSection::NoteSection(const std::string &arg_Name, Word arg_Size, Word arg_Type) : namesz{5U}, descsz{arg_Size},
                                                                                      type{arg_Type}, name{arg_Name} {
    if (arg_Name.size() + 1U != namesz) {
        Crash(); //currently only names of size 4 allowed!
    }
}

uint32_t NoteSection::CopyToBuffer(uint8_t *arg_pBuffer) const {
    uint32_t tmp_Index = 0U;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &namesz, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &descsz, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], (uint8_t *) &type, st_cOffsetWord);
    tmp_Index += st_cOffsetWord;

    uint32_t tmp_Size = name.size() + 1U;
    (void) std::memcpy(&arg_pBuffer[tmp_Index], reinterpret_cast<const uint8_t *>(name.c_str()), tmp_Size);
    tmp_Index += tmp_Size;
    tmp_Index += tmp_Size % st_cOffsetWord == 0U ? 0U : st_cOffsetWord - tmp_Size % st_cOffsetWord;

    return tmp_Index;
}
