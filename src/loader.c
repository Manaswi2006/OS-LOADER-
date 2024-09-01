#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

Elf32_Ehdr ehdr;
Elf32_Phdr phdr;
int fd;

void loader_cleanup() {
    // Clean up resources if needed
}

void* load_segment(int fd, Elf32_Phdr *phdr) {
    printf("Loading segment at offset 0x%x with size 0x%x\n", phdr->p_offset, phdr->p_memsz);

    void *segment_addr = mmap(NULL, phdr->p_memsz, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    if (segment_addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    if (lseek(fd, phdr->p_offset, SEEK_SET) == -1) {
        perror("lseek");
        exit(1);
    }

    if (read(fd, segment_addr, phdr->p_filesz) != phdr->p_filesz) {
        perror("read");
        exit(1);
    }

    if (phdr->p_memsz > phdr->p_filesz) {
        if (mmap((char*)segment_addr + phdr->p_filesz, phdr->p_memsz - phdr->p_filesz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
            perror("mmap (extended)");
            exit(1);
        }
    }

    return segment_addr;
}

void load_and_run_elf(char** exe) {
    fd = open(exe[0], O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    // Read and validate the ELF header
    if (read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
        perror("read ELF header");
        exit(1);
    }

    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 || ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 || ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "Invalid ELF file\n");
        exit(1);
    }

    printf("Entry point address: 0x%x\n", ehdr.e_entry);
    printf("Number of program headers (e_phnum): %d\n", ehdr.e_phnum);

    // Move to the PHDR table
    if (lseek(fd, ehdr.e_phoff, SEEK_SET) == -1) {
        perror("lseek to PHDR");
        exit(1);
    }

    for (int i = 0; i < ehdr.e_phnum; i++) {
        if (read(fd, &phdr, sizeof(Elf32_Phdr)) != sizeof(Elf32_Phdr)) {
            perror("read PHDR");
            exit(1);
        }

        printf("PHDR entry %d: type=0x%x, offset=0x%x, vaddr=0x%x, filesz=0x%x, memsz=0x%x\n",
               i, phdr.p_type, phdr.p_offset, phdr.p_vaddr, phdr.p_filesz, phdr.p_memsz);

        if (phdr.p_type == PT_LOAD) {
            printf("Found PT_LOAD segment.\n");

            if (ehdr.e_entry >= phdr.p_vaddr && ehdr.e_entry < (phdr.p_vaddr + phdr.p_memsz)) {
                printf("Entry point lies within this segment (vaddr=0x%x, size=0x%x).\n", phdr.p_vaddr, phdr.p_memsz);

                void *segment_addr = load_segment(fd, &phdr);
                printf("Loaded segment at address %p\n", segment_addr);

                // Address calculation
                void (*entry_point)() = (void (*)())(segment_addr + (ehdr.e_entry - phdr.p_vaddr));
                printf("Jumping to entry point at address %p\n", entry_point);

                int result = ((int (*)())entry_point)();
                printf("User _start return value = %d\n", result);

                munmap(segment_addr, phdr.p_memsz);
                break;
            }
        }
    }

    close(fd);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ELF Executable>\n", argv[0]);
        exit(1);
    }

    load_and_run_elf(&argv[1]);

    loader_cleanup();
    return 0;
}

