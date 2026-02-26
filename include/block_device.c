#include "block_device.h"
#include "../task/task.h"
#include "../task/sched.h"


int block_device_read_block(struct block_device *dev, uint32_t block_num, void *buffer) {
	struct task *task = get_current_task();
	dev->waiting_task = task;
	dev->ops->read_block(dev, block_num, buffer);
	task_block(task);

	return 0;

}


int block_device_write_block(struct block_device *dev, uint32_t block_num, void *buffer){
	struct task *task = get_current_task();
	dev->waiting_task = task;
        dev->ops->write_block(dev, block_num, buffer);
	task_block(task);

	return 0;
}

