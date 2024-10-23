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