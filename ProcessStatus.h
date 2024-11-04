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

#ifndef PROCESSSTATUS_H_
#define PROCESSSTATUS_H_

#include "NoteSection.h"
#include "Types.h"

class SigInfo {
public:
    friend class ProcessStatus;

    SigInfo();

private:
    Word signo;     // Signal number
    Word code;      // Extra code
    Word errnumber; // Errno
};

class TimeVal {
public:
    friend class ProcessStatus;

    TimeVal();

private:
    Word sec;  // Seconds
    Word usec; // Microseconds
};

class ProcessStatus {
public:
    ProcessStatus();

    uint32_t CopyAsNoteSectionToBuffer(uint8_t *arg_pBuffer, NoteSection &arg_NoteSection);

    void ReadMemory(const uint32_t *arg_pRegisters);

    static Word GetTypeForNoteSection();

private:
    SigInfo info;    // Info associated with signal
    Word cursig;  // Current signal
    Word sigpend; // Set of pending signals
    Word sighold; // Set of held signals
    Word pid;     // Process ID
    Word ppid;    // Parent's process ID
    Word pgrp;    // Group ID
    Word sid;     // Session ID
    TimeVal utime;   // User time
    TimeVal stime;   // System time
    TimeVal cutime;  // Cumulative user time
    TimeVal cstime;  // Cumulative system time
    Word reg[18]; // CPU registers
    Word fpvalid; // True if math co-processor being used
};

#endif // PROCESSSTATUS_H_

/**
 * \}
 * \}
 */
