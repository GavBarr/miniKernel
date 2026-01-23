#include "device_manager.h"
#include "strcompare.h"
#include "../debug/debug.h"
#include "../mem_alloc/heap.h"
#include <stddef.h>
#include <stdint.h>
#define MAX_NUMBER_OF_DEVICES 16

struct device_manager *manager;
static int find_first_free_slot();

int device_manager_init(){
	manager = kmalloc(sizeof(struct device_manager));
	if (manager == NULL) return -1;
	manager->count = 0;
	for (uint32_t i = 0; i < MAX_NUMBER_OF_DEVICES; i++){
		manager->devices[i] = NULL;
	}
	manager->initialized = 1;	

	return 0;

}

int register_block_device(struct block_device *device){
	if (manager->initialized != 1) return -1;
	if (manager->count == MAX_NUMBER_OF_DEVICES) return -1;
	if (device == NULL) return -1;
	if (device->block_size == 0) return -1;

	for (uint32_t i = 0; i < MAX_NUMBER_OF_DEVICES; i++){
		if (manager->devices[i] == NULL) continue;
		if (manager->devices[i]->name == NULL) continue;
		if (strcompare(manager->devices[i]->name, device->name) == 0) return -1;
	}
	
	int free_slot = find_first_free_slot();
	if (free_slot == -1) return -1;
	manager->devices[free_slot] = device;
	manager->count++;

	return 0;

}

int unregister_block_device(struct block_device *device){
        if (manager->initialized != 1) return -1;
        if (device == NULL) return -1;
        if (device->block_size == 0) return -1;
	
	print_string("\n\0");
	int device_index = -1;
	for (uint32_t i = 0; i < MAX_NUMBER_OF_DEVICES; i++){
                if (manager->devices[i] == NULL) continue;
		if (manager->devices[i]->name == NULL) continue;
		if (strcompare(manager->devices[i]->name, device->name) == 1){
			device_index = i;
			break;
		}
        }


	if (device_index == -1) return -1;
        manager->devices[device_index] = NULL;
        manager->count--;

        return 0;

}


struct block_device *find_device(char *name){
	if (manager->initialized != 1) return NULL;
	if (name == NULL) return NULL;

	for (uint32_t i = 0; i < MAX_NUMBER_OF_DEVICES; i++){
		if (manager->devices[i] == NULL) continue;
                if (manager->devices[i]->name == NULL) continue;
                if (strcompare(manager->devices[i]->name, name) != 0) return manager->devices[i];
        }

	return NULL;
	
}


static int find_first_free_slot(){
	for (int i = 0; i < MAX_NUMBER_OF_DEVICES; i++){
		if (manager->devices[i] == NULL) return i;
	}
	return -1;
}
