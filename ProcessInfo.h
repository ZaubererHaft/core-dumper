/**
 * \file
 * \{
 * \defgroup
 * \{
 * \ingroup
 * \brief
 * \details  https://github.com/torvalds/linux/blob/master/include/linux/elfcore.h
 *
 * \author   l.kratzl
 */

#ifndef HCTRLXCDPROCESSINFO_H_
#define HCTRLXCDPROCESSINFO_H_

#include "NoteSection.h"
#include "Types.h"

class ProcessInfo {
public:
    ProcessInfo();

    uint32_t CopyAsNoteSectionToBuffer(uint8_t *arg_pBuffer, NoteSection &arg_NoteSection);

    void CopyFname(const std::string &arg_String);

    void CopyPsargs(const std::string &arg_String);

    static Word GetTypeForNoteSection();

private:
    uint8_t state; /* numeric process state */
    uint8_t sname; /* char for pr_state */
    uint8_t zomb;  /* zombie */
    uint8_t nice;  /* nice val */
    Word flag;  /* flags */
    Half uid;
    Half gid;
    Word pid;
    Word ppid;
    Word pgrp;
    Word sid;
    uint8_t fname[16];  /* filename of executable */
    uint8_t psargs[80]; /* initial part of arg list */
};

#endif // HCtrlXCDPROCESSINFO_H_

/**
 * \}
 * \}
 */