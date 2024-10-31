cmake_minimum_required(VERSION 3.28)
project(core_dump)

set(CMAKE_CXX_STANDARD 17)

add_library(core-dump
        crash_dummy.cpp
        CoreWriter.cpp
        Elf.cpp
        ElfHeader.cpp
        Ident.cpp
        MemoryDump.S
        NoteSection.cpp
        ProcessInfo.cpp
        ProcessStatus.cpp
        ProgramHeader.cpp
        SectionHeader.cpp
)

target_include_directories(core-dump PUBLIC Include PRIVATE .)

