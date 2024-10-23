/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.html
 *
 * \author   l.kratzl
 */

#ifndef HCTRLXCDELFHEADER_H_
#define HCTRLXCDELFHEADER_H_

#include "Ident.h"
#include "Types.h"

enum class eDElfHeaderType_t : Half {
    ET_CORE = 4U
};

enum class eElfHeaderMachine_t : Half {
    EM_ARM = 40U
};

enum class eElfHeaderVersion_t : Word {
    EV_CURRENT = 1U
};

class ElfHeader {
public:
    ElfHeader();

public:
    uint32_t CopyToBufferAt(uint8_t *arg_pBuffer) const;

    const Ident &GetIdent() const;

    eDElfHeaderType_t GetType() const;

    eElfHeaderMachine_t GetMachine() const;

    eElfHeaderVersion_t GetVersion() const;

    Addr GetEntry() const;

    Offs GetPhoff() const;

    Offs GetShoff() const;

    Word GetFlags() const;

    Half GetEhsize() const;

    Half GetPhentsize() const;

    Half GetPhnum() const;

    Half GetShentsize() const;

    Half GetShnum() const;

    Half GetShstrndx() const;

    void SetShstrndx(Half arg_Shstrndx);

    void SetPhoff(Offs arg_Phoff);

    void SetShoff(Offs arg_Shoff);

    void IncShnum();

    void IncPhnum();

private:
    Ident ident;     // ELF identification description
    eDElfHeaderType_t type;      // Object File Type
    eElfHeaderMachine_t machine;   // required architecture for ELF
    eElfHeaderVersion_t version;   // object file version
    Addr entry;     // start address;
    Offs phoff;     // program header table file offset in bytes
    Offs shoff;     // section header table file offset in bytes
    Word flags;     // processor-specific flags
    Half ehsize;    // ELF header's size in bytes
    Half phentsize; // size in bytes of one entry program header table
    Half phnum;     // number of entries in the program header table
    Half shentsize; // section header size in bytes.
    Half shnum;     // number of entries in the section header table
    Half shstrndx;  // section header table index
};

#endif // HCtrlXCDELFHEADER_H_

/**
 * \}
 * \}
 */