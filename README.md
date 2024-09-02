# OS-LOADER


OVERVIEW
----

This project involves creating a SimpleLoader for loading and executing ELF 32-bit executables in plain C. The loader is implemented from scratch without using any external library APIs for manipulating ELF files.

PROJECT STRUCTURE
----


├── src

│      ├── loader

│      ├── loader.c

│      └── loader.h

│      └── Makefile

├── test

│      ├── Makefile

│      └── fib

│      └── fib.c

├── README.md

└── Makefile


INSTALLATION
----
To build the project, follow these steps:

    1. Clone the repository:
        git clone git@github.com:Manaswi2006/OS-LOADER-.git
    2. Install necessary software:
        Ensure you have a Unix-based OS with a C compiler and GNU make installed. For Windows, use WSL with a Linux distribution (e.g., Ubuntu).


USAGE
----
Prepare the test case:

    1. The test case fib.c should be compiled after the 'make all' command
    2. Run the loader:
        a. Use the launch executable to run the loader with the compiled ELF file as a command-line argument:
            ./src/loader ../test/fib
        b. This will execute the needful and give output of test code fib.c i.e value of fib(40)


REFERENCES
----
1. ELF Specification: https://man7.org/linux/man-pages/man5/elf.5.html
2. OSDev ELF Tutorial: https://wiki.osdev.org/ELF_Tutorial


CONTRIBUTERS
----
1. Manaswi Singh (2023307)
2. Paridhi Kotarya (2023367)
