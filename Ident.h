/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details  https://refspecs.linuxfoundation.org/elf/gabi4+/ch4.eheader.html#elfid
 *
 * \author   l.kratzl
 */

#ifndef HCTRLXCDIDENT_H_
#define HCTRLXCDIDENT_H_

#include <cstdint>

enum class eElfIdentClass_t : uint8_t {
    ELFCLASS32 = 1U
};

enum class eElfIdentData_t : uint8_t {
    ELFDATA2LSB = 1U
};

enum class eElfIdentVersion_t : uint8_t {
    EV_CURRENT = 1U
};

enum class eElfIdentOsABI_t : uint8_t {
    ELFOSABI_LINUX = 3U
};

enum class eElfIdentOsABIVersion_t : uint8_t {
    EIOSABI_UNSPECIFIED = 0U
};

class Ident {
public:
    friend class ElfHeader;

    Ident();

private:
    static constexpr uint8_t st_cMagic0 = 0x7FU;
    static constexpr uint8_t st_cMagic1 = 0x45U; // E
    static constexpr uint8_t st_cMagic2 = 0x4CU; // L
    static constexpr uint8_t st_cMagic3 = 0x46U; // F

    uint8_t data[16U];
};

#endif // HCtrlXCDIDENT_H_

/**
 * \}
 * \}
 */
