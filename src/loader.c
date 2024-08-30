#include "loader.h"
#include <sys/stat.h>

Elf32_Ehdr ehdr;
Elf32_Phdr phdr;
int fd;


void loader_cleanup() {
    //will add at end
}


void* load_segment(int fd, Elf32_Phdr *phdr) {
    printf("Loading segment at offset %u with size %u\n", phdr->p_offset, phdr->p_memsz);

    void *segment_addr = mmap(NULL, phdr->p_memsz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
            perror("mmap");
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

    // Read the ELF header
    if (read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
        perror("read");
        exit(1);
    }

    // Print entry point address
    printf("Entry point address: 0x%x\n", ehdr.e_entry);
    // According to my current debugging our code stops here
    // Iterate through the Program Header Table
    lseek(fd, ehdr.e_phoff, SEEK_SET);
    for (int i = 0; i < ehdr.e_phnum; i++) {
        if (read(fd, &phdr, sizeof(Elf32_Phdr)) != sizeof(Elf32_Phdr)) {
            perror("read");
            exit(1);
        }

        if (phdr.p_type == PT_LOAD) {
            printf("Found PT_LOAD segment.\n");

            if (ehdr.e_entry >= phdr.p_vaddr && ehdr.e_entry < (phdr.p_vaddr + phdr.p_memsz)) {
                printf("Entry point lies within this segment (vaddr=0x%x, size=0x%x).\n", phdr.p_vaddr, phdr.p_memsz);

                void *segment_addr = load_segment(fd, &phdr);
                printf("Loaded segment at address %p\n", segment_addr);

                void (*entry_point)() = (void (*)())(segment_addr + (ehdr.e_entry - phdr.p_vaddr));
                printf("Jumping to entry point at address %p\n", entry_point);

                if (mprotect(segment_addr, phdr.p_memsz, PROT_READ | PROT_EXEC) == -1) {
                    perror("mprotect");
                    exit(1);
                }

                int result = ((int (*)())entry_point)(); // Cast entry_point to function returning int
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
        printf("Usage: %s <ELF Executable> \n", argv[0]);
        exit(1);
    }

    load_and_run_elf(argv);

    loader_cleanup();
    return 0;
}

