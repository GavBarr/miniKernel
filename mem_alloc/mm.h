#ifndef MM_H
#define MM_H
#include <stdint.h>
#include "multiboot.h"

struct mmap{

        uint32_t addr;
        uint32_t len;

}__attribute__((packed));

struct mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed));


void mmap_init(void);
uint32_t get_total_entries();
void create_mmap_entries(struct mmap_entry mm_entries[]);
void copy_mmap(multiboot_info_t *mbi);
void create_mmap_entry(struct mmap_entry entries[],uint32_t index, uint32_t size, uint64_t addr, uint64_t len, uint32_t type);
uint64_t find_highest_address(struct mmap_entry entries[], uint32_t entry_count);

#endif
