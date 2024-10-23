/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details  https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.sheader.html
 *
 * \author   l.kratzl
 */

#ifndef HCTRLXCDSECTIONHEADER_H_
#define HCTRLXCDSECTIONHEADER_H_

#include "Types.h"

enum class eElfSectionType_t : Word {
    SHT_NULL = 0U,
    SHT_PROGBITS = 1U,
    SHT_STRTAB = 3U,
    SHT_NOTE = 7U,
    SHT_ARM_EXIDX = 0x70000001U
};

enum class eElfSectionFlags_t : Word {
    SHF_UNDEFINED = 0U,
    SHF_WRITE = 1U,
    SHF_ALLOC = 2U
};

class SectionHeader {
public:
    SectionHeader();

    SectionHeader(eElfSectionType_t arg_Type, Word arg_NameIndex);

    void SetOffset(Offs arg_Offset);

    void SetSize(Word arg_Size);

    void AddFlag(eElfSectionFlags_t arg_Flag);

    void SetAddr(Addr arg_Addr);

    void SetAddrAlign(Word arg_AddrAlign);

private:
    Word name;      // name of the section. Its value is an index into the section header string table section
    eElfSectionType_t type;      // This member categorizes the section's contents and semantic
    eElfSectionFlags_t flags;     // Sections support 1-bit flags that describe miscellaneous attributes
    Addr addr;      // address in memory imafe at which the section's first byte should reside.
    Offs offset;    // This member's value gives the byte offset from the beginning of the file to the first byte in the section
    Word size;      // This member gives the section's size in bytes
    Word link;      // This member holds a section header table index link,
    Word info;      // This member holds extra information, whose interpretation depends on the section type
    Word addrAlign; // address alignment constraints
    Word entsize;   // size in bytes for fixed-size entries.

public:
    uint32_t CopyToBufferAt(uint8_t *arg_pBuffer) const;
};

#endif // HCtrlXCDSECTIONHEADER_H_

/**
 * \}
 * \}
 */