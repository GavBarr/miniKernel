#include <stdint.h>
#include <stddef.h>
#include "fs.h"
#include "file.h"
#include "inode.h"
#include "superblock.h"
#include "bitmap.h"
#include "directory.h"
#include "../debug/debug.h"
#include "../include/block_device.h"
#include "../include/strlength.h"
#include "../include/strcompare.h"
#include "../mem_alloc/heap.h"
#include "../include/device_manager.h"
#include "../include/block_device.h"
#include "../drivers/ide_ata_driver.h"

#define MAX_NUMBER_IN_FILES_LIST 256
#define MAX_OPEN_FILES 256
#define FILE_TYPE 0
#define DIR_TYPE 1
static void files_in_dir_init();
static int get_file_name(char *path_name, char *file_name, uint32_t length);
static uint32_t find_number_of_components(char *path_name);
struct Superblock *sb;
struct Bitmap *inode_bitmap;
struct Bitmap *block_bitmap;
struct block_device *disk;
uint32_t root_inode_num;
struct files_in_dir *current_files_list;  


char *current_path;

struct open_file_entry open_files[MAX_OPEN_FILES];

char *get_current_path(){
	files_in_dir_init();	
	return current_path;
}

struct files_in_dir *get_files_in_dir(){
	return current_files_list;
}

int fs_change_dir(char *pathname, uint32_t flags){
	uint32_t result_inode_num;
	int status = path_resolution(root_inode_num, pathname, &result_inode_num, sb, inode_bitmap, block_bitmap, disk );
	if (status != 0) return -1;	

	struct Inode *inode = kmalloc(sizeof(struct Inode));
        inode_read(inode, result_inode_num, sb, disk);
                                        
        if(inode->type != DIR_TYPE) return -1;
	
		
	kfree(current_path);
	
	current_path = kmalloc(strlength(pathname) + 1);

	for (int i = 0; i < strlength(pathname); i++){
		current_path[i] = pathname[i];
	}
	current_path[strlength(pathname)] = '\0';


	return 0;
	
}



//return the result_inode_num, based on the path. the result_inode_num will represent the final component in the path_name
int path_resolution(uint32_t root_inode_num, char *path_name, uint32_t *result_inode_num, struct Superblock *sb, struct Bitmap *inode_bitmap, struct Bitmap *block_bitmap, struct block_device *disk){
	// /mydir/test.txt
	// [mydir, test.txt]
	if (path_name[0] != '/') return -1;

	uint32_t total_path_length = strlength(path_name);
	uint32_t current_index = 1; //not the first / after

	//uint32_t num_components = find_number_of_components(path_name);
	//if(num_components == 0) return -1;
	uint32_t cur_inode_num = root_inode_num;
		
	while(current_index < total_path_length){
		
		uint32_t length_index = current_index;
		uint32_t comp_length = 0;
		while(path_name[length_index] != '/' && path_name[length_index] != '\0'){
			length_index++;
			comp_length++;
		}
		char comp_name[comp_length + 1];
		uint32_t j = 0;
		for (uint32_t i = current_index; i < current_index + comp_length; i++){
			if (path_name[i] == '/') continue;
			comp_name[j++] = path_name[i];

		}
		comp_name[j] = '\0';
		
		uint32_t next_inode_num;
		//if (comp_name[0] == 's') print_string(comp_name);
		if (dir_lookup(cur_inode_num, comp_name, &next_inode_num, sb, disk) != 0) return -1;
		cur_inode_num = next_inode_num;
		current_index = current_index + comp_length + 1;
	}
		
	*result_inode_num = cur_inode_num;

	return 0;
}

int fd_init(void){
	for (int i = 0; i < MAX_OPEN_FILES; i++){
		struct open_file_entry entry = (struct open_file_entry)open_files[i];
		entry.inode_num = -1;
		entry.offset = 0;
		entry.flags = 0777;
		entry.in_use = 0;	
	}
}

static void files_in_dir_init(){
		
	uint32_t result_inode_num; //= kmalloc(sizeof(uint32_t));
	path_resolution(root_inode_num, current_path, &result_inode_num, sb, inode_bitmap, block_bitmap, disk);
	struct Inode *inode = kmalloc(sizeof(struct Inode));
        inode_read(inode, result_inode_num, sb, disk);	
		if (inode->data_blocks[0] == -1) return;
		uint8_t block_buf[sb->block_size];
		disk->ops->read_block(disk, inode->data_blocks[0], block_buf);

		uint32_t offset = 0;
		uint32_t files_index = 0;

		while(offset < inode->size){
			struct dir_entry *entry = (struct dir_entry *)(block_buf + offset);
			char *name = (char *)(entry + 1); //remember that the name is stored right after the dir_entry	
			current_files_list[files_index].file_name = name;
			current_files_list[files_index].size = strlength(name);
			current_files_list[files_index].type = entry->file_type;
			//print_string("file_type->\0");
			//print_int(entry->file_type);
			files_index++;
			offset += entry->entry_length;
		}

	kfree(inode);
}

int fs_mkdir(char *path_name, uint32_t flags){
	
	uint32_t result_inode_num;      
        int resolution_status = path_resolution(root_inode_num, path_name, &result_inode_num, sb, inode_bitmap, block_bitmap, disk);
        if (resolution_status == -1){
                uint32_t length = 0;

                for (int i = strlength(path_name) - 1; i >= 0; i--){
                        if (path_name[i] == '/'){
                                length = strlength(path_name) - i - 1;
                                break;
                        }
                }

                char *dir_name = kmalloc(length + 1);
                get_file_name(path_name, dir_name, length);
		//print_string("dir->\0");
		//print_string(dir_name);
		if (dir_create(result_inode_num, dir_name, flags, inode_bitmap, block_bitmap,sb,disk) != 0) return -1;
                //print_string("SUCCESFULLY CREATED FILE!\n\0");
                if (path_resolution(root_inode_num, path_name, &result_inode_num, sb, inode_bitmap, block_bitmap, disk) == -1) return -1;
		//print_int(result_inode_num);
		kfree(dir_name);
        }else{
		return -1;
	}
	
	
	return 0;
}


void fs_init(void){
//	open_files[MAX_OPEN_FILES] = kmalloc(sizeof(struct open_file_entry) * MAX_OPEN_FILES);
	current_files_list = kmalloc(sizeof(struct files_in_dir) * MAX_NUMBER_IN_FILES_LIST);
	current_path = kmalloc(2);
	current_path[0] = '/';
	current_path[1] = '\0';
	
	fd_init();
	disk = ide_init();
	register_block_device(disk);

	uint32_t disk_size = disk->block_size * disk->block_count;
        sb = kmalloc(sizeof(struct Superblock));
        inode_bitmap = kmalloc(sizeof(struct Bitmap));
        block_bitmap = kmalloc(sizeof(struct Bitmap));
        int test_file_creation = 0;

        superblock_init(sb, disk_size);
        fs_bitmap_init(inode_bitmap, 256);
        fs_bitmap_init(block_bitmap, sb->total_blocks);

        for (uint32_t i = 0; i < sb->data_blocks_start; i++) {
                fs_bitmap_set(block_bitmap, i);
        }
        sb->free_blocks = sb->total_blocks - sb->data_blocks_start;
        //create directory testing
        file_create(sb, &root_inode_num, inode_bitmap, 0777, disk);
        struct Inode *root_inode = kmalloc(sizeof(struct Inode));
        inode_read(root_inode, root_inode_num, sb, disk);


        root_inode->type = DIR_TYPE;
        inode_write(root_inode, root_inode_num, sb, disk);


        dir_add_entry(root_inode, root_inode_num, ".", root_inode_num, DIR_TYPE, disk, sb, block_bitmap);
        inode_read(root_inode, root_inode_num, sb, disk);
        dir_add_entry(root_inode, root_inode_num, "..", root_inode_num, DIR_TYPE, disk, sb, block_bitmap);
}

int fs_open(char *path_name, uint32_t flags){
	uint32_t result_inode_num;	
	int resolution_status = path_resolution(root_inode_num, path_name, &result_inode_num, sb, inode_bitmap, block_bitmap, disk);
	if (resolution_status == -1){
		uint32_t length = 0;

        	for (int i = strlength(path_name) - 1; i >= 0; i--){
	                if (path_name[i] == '/'){
	                        length = strlength(path_name) - i - 1;
                        	break;
                	}
        	}

		char *file_name = kmalloc(length + 1);
	        get_file_name(path_name, file_name, length);
		
		//print_string(file_name);
		if (file_create_inside_dir(root_inode_num, file_name, 0777, inode_bitmap, block_bitmap, sb, disk) != 0) return -1;
		
		//print_string("SUCCESFULLY CREATED FILE!\n\0");
		if (path_resolution(root_inode_num, path_name, &result_inode_num, sb, inode_bitmap, block_bitmap, disk) == -1) return -1;
	}

	

	struct Inode *inode = kmalloc(sizeof(struct Inode));
	inode_read(inode, result_inode_num, sb, disk);
	if (inode->type == DIR_TYPE) return -1;

	//VALIDATE ACCESS TODO
	for(int i = 0; i < MAX_OPEN_FILES; i++){
		if (open_files[i].in_use == 0){
			open_files[i].inode_num = result_inode_num;
			open_files[i].offset = 0;
			open_files[i].flags = 0777;
			open_files[i].in_use = 1; 
				
			return i;	
		}

	}	
	

	return -1;
}

int fs_close(uint32_t entry_index){

	if (entry_index < 0 || entry_index > MAX_OPEN_FILES) return -1;
	if (open_files[entry_index].in_use == 0) return -1;

		
	open_files[entry_index].inode_num = -1;
	open_files[entry_index].offset = 0;
	open_files[entry_index].flags = 0777;
	open_files[entry_index].in_use = 0;

	return 0;


}


int fs_write(uint32_t entry_index, char *buf, uint32_t size){

	if (entry_index < 0 || entry_index > MAX_OPEN_FILES) return -1;
	if (open_files[entry_index].in_use == 0) return -1;
	if (size == 0) return -1;

        struct Inode *inode = kmalloc(sizeof(struct Inode));
	uint32_t inode_num = open_files[entry_index].inode_num;	
	uint32_t offset = open_files[entry_index].offset;	


        if(inode_read(inode, inode_num, sb, disk) != 0){
		kfree(inode);
		return -1;
	}
        if(file_write(inode, inode_num, buf, offset, size, block_bitmap, sb, disk) != 0){
		kfree(inode);
		return -1;
	}	

	uint32_t bytes_written = size;
	open_files[entry_index].offset += bytes_written;	
	
	kfree(inode);
	return 0;
}



int fs_read(uint32_t entry_index, char *buf, uint32_t size){

	if (entry_index < 0 || entry_index > MAX_OPEN_FILES) return -1;
        if (open_files[entry_index].in_use == 0) return -1;
	if (size == 0) return -1;
	struct Inode *inode = kmalloc(sizeof(struct Inode));
	uint32_t inode_num = open_files[entry_index].inode_num;
	uint32_t offset = open_files[entry_index].offset;

	if(inode_read(inode, inode_num, sb, disk) != 0){ 
                kfree(inode);
                return -1;
        }

	if(file_read(inode, inode_num, buf, offset, size, sb, disk) == 0){
		kfree(inode);
		return -1;	
	}
	uint32_t bytes_read = size;
	open_files[entry_index].offset += bytes_read;

	return 0;
}


static int get_file_name(char *path_name, char *file_name, uint32_t length){
		
	uint32_t i = strlength(path_name) - 1;
	uint32_t j = length - 1;
	file_name[length] = '\0';
	while (path_name[i] != '/' && i >= 0 ){
		file_name[j] = path_name[i];
		j--; 
		i--;
	}

	
	return 0;	
	

}


static uint32_t find_number_of_components(char *path_name){

	uint32_t current_index = 0;
	uint32_t number_of_comps = 0;
	uint32_t total_path_length = strlength(path_name);


	while(current_index < total_path_length){

		if (path_name[current_index] == '/') number_of_comps++;
		current_index++;	
	}

	return number_of_comps;
}
