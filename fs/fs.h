#ifndef FS_H
#define FS_H
#define MAX_OPEN_FILES 256

#include <stdint.h>
#include "superblock.h"
#include "bitmap.h"
#include "../include/block_device.h"

struct open_file_entry{
	uint32_t inode_num;
	uint32_t offset;
	uint32_t flags; //0777, 0677
	int in_use;
};

struct files_in_dir{
	char *file_name;
	uint32_t size;
	uint32_t type;

};

//struct file_descriptior_table{
//	struct open_file_entry[MAX_OPEN_FILES];
//};

char *get_current_path();
struct files_in_dir *get_files_in_dir();
int path_resolution(uint32_t root_inode_num, char *path_name, uint32_t *result_inode_num, struct Superblock *sb, struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap, struct block_device *disk);
int fs_open(char *path_name, uint32_t flags);
int fs_close(uint32_t entry_index);
int fs_write(uint32_t entry_index, char *buf, uint32_t size);
int fs_read(uint32_t entry_index, char *buf, uint32_t size);
int fd_init(void);
void fs_init(void);

#endif
