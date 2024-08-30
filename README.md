# OS-LOADER


OVERVIEW
----

This project involves creating a SimpleLoader for loading and executing ELF 32-bit executables in plain C. The loader must be implemented from scratch without using any external library APIs for manipulating ELF files. The resulting code will be compiled into a shared library (lib_simpleloader.so) and used to load and run executable files through a helper program.


PROJECT STRUCTURE
----

main/ 

│

├── bin/
 
│   ├── launch        # Executable for running the loader 

│   └── lib_simpleloader.so # Compiled shared library 

│ 

├── src/ 

│   ├── loader.c     # Implementation of the SimpleLoader 

│   └── launch.c     # Helper program for executing the loader 

│ 

├── test/ 

│   └── fib.c        # Test case for the loader 	

│ 

├── Makefile         # Top-level Makefile to build the project 

├── README.md        # This file 

└── design_document.pdf # Design document detailing the implementation and contributions



INSTALLATION
----
To compile and build the project, follow these steps:

    1. Clone the repository:
        git clone git@github.com:Manaswi2006/OS-LOADER-.git
    2. Install necessary software:
        Ensure you have a Unix-based OS with a C compiler and GNU make installed. For Windows, use WSL with a Linux distribution (e.g., Ubuntu).
    3. Compile the project:
        make
        This will compile the loader.c into a shared library (lib_simpleloader.so) and the launch.c into an executable (launch).


USAGE
----
Prepare the test case:

    1. The test case fib.c should be compiled using the following command:
        a. gcc -m32 -no-pie -nostdlib -o test/fib test/fib.c
    2. Run the loader:
        a. Use the launch executable to run the loader with the compiled ELF file as a command-line argument:
            i. ./bin/launch test/fib
        b. This will load and execute the fib ELF file using lib_simpleloader.so.


REFERENCES
----
1. ELF Specification: https://man7.org/linux/man-pages/man5/elf.5.html
2. OSDev ELF Tutorial: https://wiki.osdev.org/ELF_Tutorial


CONTRIBUTERS
----
1. Manaswi Singh (2023307)
2. Paridhi Kotaraya (2023367)
