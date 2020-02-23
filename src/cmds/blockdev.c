/**
 * @file
 *
 * @date 20.02.23
 * @author Adityar Rayala
 */

#include<stdio.h>
#include<string.h>
#include<assert.h>
#include <drivers/block_dev.h>

static void print_usage(void) {
    printf("Usage:\nblockdev command devices\n");
    printf("blockdev --report [devices]\n");
    printf("Options:\n");
    printf(" %-15s print report for specified (or all) devices.\n", "--report");
    printf("Available Commands:\n");
    printf(" %-15s get size in 512-byte sectors.\n", "--getbsz");
}

static inline struct block_dev* chk_dev_avail(char* dev_name) {
	struct block_dev *bdev;

	bdev = block_dev_find(dev_name);
	if (!bdev) {
		printf("Block device \"%s\" not found.\n", dev_name);
		return 0;
	}

	return bdev;
}

int main(int argc, char **argv) {
    struct block_dev **bdevs, *bdev;

    if (argc < 2) {
        print_usage();
        return 0;
    }

	if (!strcmp(argv[1], "--getbsz")) {
			if (argc < 3){
				printf("blockdev: No device specified\n");
			}
			else{
				bdev = chk_dev_avail(argv[2]);
				if (bdev) {
					printf("%d\n", bdev->block_size);
				}
			}
	}
	else if (!strcmp(argv[1], "--report")) {
		printf("------------------------------------------------------------\n");
		printf("| %-15s | %-15s | %-20s |\n", "Device Name", "BSZ", "Size");
		printf("------------------------------------------------------------\n");
		bdevs = get_bdev_tab();
		assert(bdevs);
		if (argc >= 3){
			bdev = chk_dev_avail(argv[2]);
			if (bdev) {
				printf("| %-15s | %-15d | %-20lld |\n", bdev->name, bdev->block_size, bdev->size);
				printf("------------------------------------------------------------\n");
			}
		}
		else {
			bdev = *(bdevs);
			while(bdev) {
				printf("| %-15s | %-15d | %-20lld |\n", bdev->name, bdev->block_size, bdev->size);
				bdev = *(++bdevs);
			}
			printf("------------------------------------------------------------\n");
		}
	}
}
