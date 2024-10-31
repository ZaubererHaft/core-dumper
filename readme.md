# ELF Core Writer for ARM-Cortex M processors

This library can be used to create an ELF core dump file into a provided buffer. When the file is written to the file system it can be later analyzed using `arm-gdb`.
With that, stack traces can be generated for post-mortem analyses.

## How To Use
The following steps should be executed with disabled interrupts. 

1) Create a memory info instance: `MemoryInformation memInfo{stackEnd, stackSize, dataEnd, dataSize}`. The object needs the end addresses of the stack and data segment as well as their size. This information can usually be obtained using the linker script (`*.ld`)
2) Create an instance of the core writer and pass the memory Info: `CoreWriter writer{memInfo}`
3) Allocate a memory buffer, e.g. using `new`. The size of the buffer should match the size of the stack and data segment as well as additional bytes for the elf headers
4) Call `writer.Write(buffer, buffLen)`. The elf-file is now stored in the buffer
5) Now write the stack trace to the file system

## Analyze the core-dump file
* Start the `arm-none-eabi-gdb` console
* Load your executable elf: `file exec.elf`
* Now load the core-dump elf: `core-file core-dump.elf`
* *Important: If another core-file should be loaded, restart GDB!*

### Useful commands
* `bt` - create the stack trace
* `up / down` - navigate through the stack trace
* `info register` - shows the register values of the active frame
* `info args` - shows the function arguments of the active frame
* `info frame` - shows important values of the active frame

### Troubleshooting / Analyze core-file
Use the `eu-readelf` to analyze the core dump file in case of problems. The command `eu-readelf -a coredump.elf` should produce an output like this (Note that the context / note section is not part of the current version):

![alt text](img.png "Image")

If not, the file might be corrupted.

## Add a section header
Sometimes, custom or additional section should be added. Perform the following steps to do so: 

1) increase CoreWriter::st_cCountSectionHeaders by 1
2) add name of section to CoreWriter::st_cStringTable
3) add index to name in string table
4) add index to store the position of the section in the elf file
5) add section to CoreWriter::createSectionHeaders similar to the existing sections
6) if required, add the payload as byte array (see e.g. CoreWriter::createNoteSectionsPayload)
7) link header and payload using Elf::AddGenericPayload
8) add the payload size in CoreWriter::GetMaxRequiredNumberOfBytes
9) make sure to extend the allowed number of sections and payloads in Elf

Similar steps are to be done when adding a program header.

## Receive stack / data addresses
Typically, these sections are defined in a linker file.  E.g., STM32 Nucleo defines the following data section
```
_  .data :
{
. = ALIGN(4);
_sdata = .;        /* create a global symbol at data start */
*(.data)           /* .data sections */
*(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */

} >RAM 
```
Here, we can use the `_sdata` and `_edata` symbols to create the memory information. if they are defined for the stack as well, one can use the following code to load them in the code:
```
extern char DataStart asm("_sdata");
extern char DataEnd asm("_edata");
extern char StackEnd asm("_estack");
extern char StackStart asm("_sstack");

MemoryInformation memInfo{(uint32_t)&StackStart, (uint32_t)&StackEnd, (uint32_t) &DataStart, (uint32_t) &DataEnd};
```

If the symbols do no exist, add them in the linker file or read the addresses form the sections in the `elf` file using `eu-readelf`. 