/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details  https://refspecs.linuxbase.org/elf/gabi4+/ch5.pheader.html
 *
 * \author   l.kratzl
 */

#ifndef PROGRAMHEADER_H_
#define PROGRAMHEADER_H_

#include "Types.h"

enum class eElfProgramType_t : Word {
    PT_NULL = 0U,
    PT_LOAD = 1U,
    PT_NOTE = 4U,
    PT_ARM_EXIDX = 0x70000001U
};

enum class eElfProgramPermissions_t : Word {
    NONE,
    PF_X,
    PF_W,
    PF_W_X,
    PF_R,
    PF_R_X,
    PF_R_W,
    PF_WWX
};

class ProgramHeader {
public:
    ProgramHeader();

    ProgramHeader(eElfProgramType_t arg_Type, eElfProgramPermissions_t arg_Flag);

    uint32_t CopyToBufferAt(uint8_t *arg_pBuffer) const;

    void SetOffset(Offs arg_Offset);

    void SetFilesize(Word arg_Filesize);

    void SetMemsize(Word arg_Memsize);

    void SetVaddr(Addr arg_Vaddr);

    void SetPaddr(Addr arg_Paddr);

    void SetAlign(Word arg_Align);

private:
    eElfProgramType_t type;     // array element information.
    Offs offset;   // offset from the beginning of the file at which the first byte of the segment resides.
    Addr vaddr;    // virtual address at which the first byte of the segment resides in memory.
    Addr paddr;    // reserved for the segment's physical address
    Word filesize; // number of bytes in the file image of the segment
    Word memsize;  // number of bytes in the memory image of the segment
    eElfProgramPermissions_t flags;    // flags relevant to the segment
    Word align;    // value to which the segments are aligned in memory and in the file
};

#endif // PROGRAMHEADER_H_

/**
 * \}
 * \}
 */