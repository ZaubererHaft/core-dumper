/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details  https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter6-18048.html
 *
 * \author   l.kratzl
 */

#ifndef NOTESECTION_H_
#define NOTESECTION_H_

#include "Types.h"
#include <string>

class NoteSection {
public:
    NoteSection();

    NoteSection(const std::string &arg_Name, Word arg_Size, Word arg_Type);

    uint32_t CopyToBuffer(uint8_t *arg_pBuffer) const;

private:
    Word namesz;
    Word descsz;
    Word type;
    std::string name;
};

#endif // NOTESECTION_H_

/**
 * \}
 * \}
 */
