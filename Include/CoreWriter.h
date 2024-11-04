/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief    Class to write a core file at runtime
 * \details
 * \author   l.kratzl
 */

#ifndef HCTRLXCDCOREWRITER_H_
#define HCTRLXCDCOREWRITER_H_

#include "Elf.h"
#include "ProcessInfo.h"
#include "ProcessStatus.h"
#include "MemoryInformation.h"

class CoreWriter {
public:
    /**
     * \brief Creates a new instance of the core writer
     */
    explicit CoreWriter(const MemoryInformation &arg_MemInfo);

    /**
     * \brief Writes a core file into \p arg_pBuffer.
     * \param arg_pBuffer Pointer to the buffer where the core is written to
     * \param arg_BuffLen The length of \p arg_pBuffer to check if the space is sufficient.
     */
    uint32_t Write(uint8_t *arg_pBuffer, uint32_t arg_BuffLen);

    /**
     * \brief Returns the maximum number of bytes th final core file will use
     * \details As there is a dynamic note section, the size can differ from project to project.
     */
    uint32_t GetMaxRequiredNumberOfBytes() const;

private:
    static constexpr uint32_t st_cCountSectionHeaders = 5U; // the number of section headers that will be written in the elf file
    static constexpr uint32_t st_cCountProgramHeaders = 3U; // the number of program headers that will be written in the elf file

    static constexpr const char st_cStringTable[] = "\0.shstrtab\0note0\0.data\0Stack\0"; // the string table of the elf file
    static constexpr uint32_t st_cStringTableSize = sizeof(st_cStringTable); // the size of the string table NOLINT(*-sizeof-expression)

    static constexpr uint32_t st_cSingleNoteSize = 20U; // fixed size of any note section (limited to 5 Byte payload)
    static constexpr uint32_t st_cNoteSectionSize =      // size of the .note section
            sizeof(ProcessInfo) + sizeof(ProcessStatus) + 2U * st_cSingleNoteSize;

    // The indices of the names in the string table (manually calculated)
    static constexpr uint8_t st_cIndexStringTableNameInStringTable = 1U;
    static constexpr uint8_t st_cIndexNoteSectionNameInStringTable = 11U;
    static constexpr uint8_t st_cIndexStackSectionNameInStringTable = 23U;
    static constexpr uint8_t st_cIndexDataSectionNameInStringTable = 17U;

    // stores where the index of the section header in the elf file is located
    static constexpr uint8_t st_cIndexNullSectionHeader = 0U;
    static constexpr uint8_t st_cIndexStringSectionHeader = 1U;
    static constexpr uint8_t st_cIndexNoteSectionHeader = 2U;
    static constexpr uint8_t st_cIndexStackSectionHeader = 3U;
    static constexpr uint8_t st_cIndexDataSectionHeader = 4U;

    // stores where the index of the program header in the elf file is located
    static constexpr uint8_t st_cIndexNoteProgramHeader = 0U;
    static constexpr uint8_t st_cIndexStackProgramHeader = 1U;
    static constexpr uint8_t st_cIndexDataProgramHeader = 2U;

    /**
     * \brief Creates the elf file where section and program headers are then written to.
     *        Sets in addition the index of the string table section in the section headers.
     */
    static Elf createElf();

    /**
     * \brief Returns the number of bytes in the elf which is not payload but header information
     *        such as section headers and program headers and the size of the elf header.
     */
    static constexpr uint32_t getNumberOfBytesUntilPayload();

    void createSectionHeaders(uint8_t arg_pSectionHeaderIndices[st_cCountSectionHeaders]);

    void createProgramHeaders(uint8_t arg_pSrogramHeaderIndices[st_cCountProgramHeaders]);

    static uint32_t createNoteSectionsPayload(uint8_t *arg_pBuffer, uint32_t *arg_pRegisters);

    MemoryInformation memInfo;
    Elf elf;
    uint32_t addresses[4];
};

#endif // HCTRLXCDCOREWRITER_H_

/**
 * \}
 * \}
 */
